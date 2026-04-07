/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:58:29 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/06 07:53:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void monitor_routine(t_monitor *ctx)
{
	int			i;
	long long	deadline;
	long long	now;

	i = 0;
	while (1)
	{
		if (is_sim_stopped(ctx->sim))
		return (NULL);
		
		ctx->coders_finished = 0;
		i = 0;
		while (i < ctx->sim->n_coders)
		{
			pthread_mutex_lock(&ctx->coders[i].state_mutex);
			deadline = (ctx->coders[i].last_compile
					+ ctx->sim->time_to_burnout_ms);
			if (ctx->coders[i].compile_count
				== ctx->sim->number_of_compiles_required)
				ctx->coders_finished += 1;
			pthread_mutex_unlock(&ctx->coders[i].state_mutex);

			now = get_now_in_ms();
			if (now > deadline)
			{
				set_stop_flag(ctx->sim);
				pthread_mutex_lock(&ctx->sim->output_mutex);
				printf("%lld %i has burned out",
					now - ctx->sim->start, ctx->coders[i].id);
				pthread_mutex_unlock(&ctx->sim->output_mutex);
			}
			i++;
		}
		if (ctx->coders_finished == ctx->sim->n_coders)
		{
			set_stop_flag(ctx->sim);
			return(NULL);
		}
		usleep(1000);
	}
}

int	is_sim_stopped(t_sim *sim)
{
	int	stopped;
	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop_flag;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}

void	set_stop_flag(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}
