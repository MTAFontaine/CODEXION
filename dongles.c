/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:43:09 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/07 08:33:13 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	acquire_dongles(t_coders *coder)
{
	t_dongle		*first;
	t_dongle		*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (is_sim_stopped(coder->sim))
		return ;
	get_dongle(first, coder);
	if (is_sim_stopped(coder->sim))
	{
		release_dongle(first);
		return ;
	}
	get_dongle(second, coder);
	if (is_sim_stopped(coder->sim))
	{
		release_dongle(first);
		release_dongle(second);
		return ;
	}
}

void	get_dongle(t_dongle *d, t_coders *coder)
{
	long long		now;
	struct timespec	ts;

	pthread_mutex_lock(&d->mutex);
	scheduler_enqueue(d, coder);
	while (d->used || peek(&d->queue) != coder->id
		|| d->available_time > get_now_in_ms())
	{
		now = get_now_in_ms();
		if (!d->used && peek(&d->queue) == coder->id && d->available_time > now)
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
	pthread_mutex_lock(&coder->sim->output_mutex);
	printf("%lld %i has taken a dongle\n",
		(get_now_in_ms() - coder->sim->start), coder->id);
	pthread_mutex_unlock(&coder->sim->output_mutex);
}

void	release_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->used = 0;
	d->available_time = get_now_in_ms() + d->cooldown_time;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}

void	broadcast_all_dongles(t_dongle *dongles, int n_coders) {
	for (int i = 0; i < n_coders; ++i) {
		pthread_mutex_lock(&dongles[i].mutex);
		pthread_cond_broadcast(&dongles[i].cond);
		pthread_mutex_unlock(&dongles[i].mutex);
	}
}
