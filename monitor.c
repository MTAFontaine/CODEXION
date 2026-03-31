/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:58:29 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/31 12:17:08 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void monitor_routine(t_monitor *ctx)
{
	int			i;
	long long	deadline;

	i = 0;
	while (1)
	{
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
			if (get_now_in_ms() > deadline)
			{
				set_stop_flag(*ctx->sim);
				pthread_mutex_lock(&ctx->sim->output_mutex);
				printf("%lld %i has burned out",
					get_now_in_ms(), ctx->coders[i].id);
				pthread_mutex_unlock(&ctx->sim->output_mutex);
			}
			if (ctx->coders_finished == ctx->sim->n_coders)
			{
				
			}
			i++;
			usleep(1000);
		}
	}
}

int	is_sim_stopped(t_sim sim)
{
	return (sim.stop_flag);
}

void	set_stop_flag(t_sim sim)
{
	pthread_mutex_lock(&sim.stop_mutex);
	sim.stop_flag = 1;
	pthread_mutex_unlock(&sim.stop_mutex);
}
