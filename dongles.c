/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:43:09 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/31 12:13:14 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
