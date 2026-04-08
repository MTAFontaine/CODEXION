/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:24:05 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/08 12:08:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
TODO: Parse argument and populate simulation struct
*/


int	main(int argc, char *argv[])
{
	int			i;
	t_sim		sim;
	t_coders	*coders;
	t_dongle	*dongles;
	t_monitor	monitor_ctx;
	pthread_t	monitor_thread;

	if (validate_arguments(argc, argv, &sim))
	{
		printf("Error\n");
		return (0);
	}
	sim.start = get_now_in_ms();
	i = 0;
	coders = malloc(sizeof (t_coders) * sim.n_coders);
	dongles = malloc(sizeof (*dongles) * sim.n_coders);
	init_dongles(dongles, &sim);
	init_coders(&sim, dongles, coders);
	init_monitor(&monitor_ctx, dongles, coders, &sim);

	pthread_create(&monitor_thread, NULL, monitor_routine, &monitor_ctx);

	i = 0;
	while (i < sim.n_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}

	pthread_join(monitor_thread, NULL);
	
	cleanup_queues(dongles, sim.n_coders);
	cleanup_mutexes(&sim, coders, dongles);
	free(coders);
	free(dongles);
}

