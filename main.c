/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:24:05 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/07 08:20:00 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
TODO: Parse argument and populate simulation struct
*/


int	main(void)
{
	int			i;
	t_sim		sim;
	t_coders	*coders;
	t_dongle	*dongles;
	t_monitor	monitor_ctx;
	pthread_t	monitor_thread;

	i = 0;
	sim.n_coders = 5;
	sim.start = get_now_in_ms();
	sim.time_to_burnout_ms = 7;
	sim.time_to_compile_ms = 10;
	sim.time_to_debug_ms = 100;
	sim.time_to_refactor_ms = 100;
	sim.number_of_compiles_required = 3;
	sim.dongle_cooldown_ms = 100;
	sim.scheduler = 0;
	sim.stop_flag = 0;
	pthread_mutex_init(&sim.output_mutex, NULL);
	pthread_mutex_init(&sim.stop_mutex, NULL);

	coders = malloc(sizeof (t_coders) * sim.n_coders);
	dongles = malloc(sizeof (*dongles) * sim.n_coders);
	init_dongles(dongles, &sim);
	init_coders(&sim, dongles, coders);

	monitor_ctx.coders_finished = 0;
	monitor_ctx.dongles = dongles;
	monitor_ctx.coders = coders;
	monitor_ctx.sim = &sim;

	pthread_create(&monitor_thread, NULL, monitor_routine, &monitor_ctx);

	i = 0;
	while (i < sim.n_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}

	pthread_join(monitor_thread, NULL);

	free(coders);
	free(dongles);
}

