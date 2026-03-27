/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:45 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/27 10:32:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <time.h>

void	append(t_fifo_queue	*queue, int coder_id)
{
	t_queue_node	*new_node;

	new_node = malloc(sizeof(t_queue_node));
	if (!new_node)
		exit(-1);
	new_node->coder_id = coder_id;
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

void	pop_head(t_fifo_queue *queue)
{
	t_queue_node	*old_head;

	if (!queue || !queue->head)
		return ;
	old_head = queue->head;
	queue->head = old_head->next;
	if (queue->head == NULL)
		queue->tail = NULL;
	free(old_head);
}

int	peek(t_fifo_queue	*queue)
{
	if (queue->head == NULL)
		return (-1);
	return (queue->head->coder_id);
}

void	get_dongle(t_dongle *d, t_coders coder)
{
	long long		now;
	struct timespec	ts;

	pthread_mutex_lock(&d->mutex);
	append(&d->fifo, coder.id);
	while (d->used || peek(&d->fifo) != coder.id
		|| d->available_time > get_now_in_ms())
	{
		now = get_now_in_ms();
		if (!d->used && peek(&d->fifo) == coder.id && d->available_time > now)
		{
			ts.tv_sec = d->available_time / 1000;
			ts.tv_nsec = (d->available_time % 1000) * 1000000;
			pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
		}
		else
			pthread_cond_wait(&d->cond, &d->mutex);
	}
	d->used = 1;
	pop_head(&d->fifo);
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
