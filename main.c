/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:24:05 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/14 08:10:59 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	setup_simulation(t_sim *sim, t_coders **coders,
	t_dongle **dongles, t_monitor *ctx)
{
	*coders = malloc(sizeof (t_coders) * sim->n_coders);
	*dongles = malloc(sizeof (**dongles) * sim->n_coders);
	if (!*coders || !*dongles)
	{
		free(*coders);
		free(*dongles);
		return (1);
	}
	init_dongles(*dongles, sim);
	init_coders(sim, *dongles, *coders);
	init_monitor(ctx, *dongles, *coders, sim);
	pthread_create(&ctx->m_thread, NULL, monitor_routine, ctx);
	return (0);
}

static void	join_threads(t_monitor *ctx, t_coders *coders, int n_coders)
{
	int	i;

	i = 0;
	while (i < n_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}
	pthread_join(ctx->m_thread, NULL);
}

int	main(int argc, char *argv[])
{
	t_sim		sim;
	t_coders	*coders;
	t_dongle	*dongles;
	t_monitor	ctx;

	if (validate_arguments(argc, argv, &sim))
	{
		printf("Error\n");
		return (0);
	}
	sim.start = get_now_in_ms();
	if (setup_simulation(&sim, &coders, &dongles, &ctx))
		return (1);
	join_threads(&ctx, coders, sim.n_coders);
	cleanup_queues(dongles, sim.n_coders);
	cleanup_mutexes(&sim, coders, dongles);
	free(coders);
	free(dongles);
	return (0);
}
