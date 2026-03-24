/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:24:05 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/24 16:23:21 by mafontai         ###   ########.fr       */
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

	i = 0;
	sim.n_coders = 25;
	sim.start = get_now_in_ms();
	sim.time_to_burnout_ms = 700;
	sim.time_to_compile_ms = 100;
	sim.time_to_debug_ms = 100;
	sim.time_to_refactor_ms = 100;
	sim.number_of_compiles_required = 3;
	sim.dongle_cooldown_ms = 100;
	sim.scheduler = 1;

	coders = malloc(sizeof (t_coders) * sim.n_coders);
	dongles = malloc(sizeof (*dongles) * sim.n_coders);
	init_dongles(dongles, sim);
	init_coders(&sim, dongles, coders);

	i = 0;
	while (i < sim.n_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}
	free(coders);
	free(dongles);
}

