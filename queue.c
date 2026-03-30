/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:45 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/30 15:44:32 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <time.h>

static int	is_before(long long deadline_ms, int coder_id, t_queue_node *node)
{
	return (deadline_ms < node->deadline_ms
		|| (deadline_ms == node->deadline_ms && coder_id < node->coder_id));
}

void	append_fifo(t_queue	*queue, int coder_id, long long deadline_ms)
{
	t_queue_node	*new_node;

	new_node = malloc(sizeof(t_queue_node));
	if (!new_node)
		exit(-1);
	new_node->coder_id = coder_id;
	new_node->deadline_ms = deadline_ms;
	new_node->next = NULL;

	if (queue->tail == NULL)
	{
		queue->head = new_node;
		queue->tail = new_node;
	}
	else
	{
		queue->tail->next = new_node;
		queue->tail = new_node;
	}
}

void	prepend_edf(t_queue *queue, int coder_id, long long deadline_ms)
{
	t_queue_node	*new_node;
	t_queue_node	*cur;

	new_node = malloc(sizeof(t_queue_node));
	if (!new_node)
		exit(-1);
	new_node->coder_id = coder_id;
	new_node->deadline_ms = deadline_ms;
	new_node->next = NULL;
	if (!queue->head || is_before(deadline_ms, coder_id, queue->head))
	{
		if (!queue->head)
			queue->tail = new_node;
		else
			new_node->next = queue->head;
		queue->head = new_node;
		return ;
	}
	cur = queue->head;
	while (cur->next && !is_before(deadline_ms, coder_id, cur->next))
		cur = cur->next;
	new_node->next = cur->next;
	cur->next = new_node;
	if (new_node->next == NULL)
		queue->tail = new_node;
}

void	get_dongle(t_dongle *d, t_coders coder)
{
	long long		now;
	struct timespec	ts;

	pthread_mutex_lock(&d->mutex);
	scheduler_enqueue(d, coder);
	while (d->used || peek(&d->queue) != coder.id
		|| d->available_time > get_now_in_ms())
	{
		now = get_now_in_ms();
		if (!d->used && peek(&d->queue) == coder.id && d->available_time > now)
		{
			ts.tv_sec = d->available_time / 1000;
			ts.tv_nsec = (d->available_time % 1000) * 1000000;
			pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
		}
		else
			pthread_cond_wait(&d->cond, &d->mutex);
	}
	d->used = 1;
	pop_head(&d->queue);
	pthread_mutex_unlock(&d->mutex);
	pthread_mutex_lock(&coder.sim->output_mutex);
	printf("%lld %i has taken a dongle\n",
		(get_now_in_ms() - coder.sim->start), coder.id);
	pthread_mutex_unlock(&coder.sim->output_mutex);
}

void	release_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->used = 0;
	d->available_time = get_now_in_ms() + d->cooldown_time;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
