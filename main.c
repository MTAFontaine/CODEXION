/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:24:05 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/10 13:42:58 by mafontai         ###   ########.fr       */
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
	sim.n_coders = 3;
	sim.time_to_burnout_ms = 10000;
	sim.time_to_compile_ms = 500;
	sim.time_to_debug_ms = 500;
	sim.time_to_refactor_ms = 500;
	sim.number_of_compiles_required = 3;
	sim.dongle_cooldown_ms = 500;
	sim.scheduler = 1;

	coders = malloc(sizeof (t_coders) * sim.n_coders);
	dongles = malloc(sizeof (*dongles) * sim.n_coders);
	while (i < sim.n_coders)
	{
		pthread_mutex_init(&dongles[i].mutex, NULL);
		dongles[i].id = i;
		dongles[i].used = 0;
		dongles[i].cooldown_time = sim.dongle_cooldown_ms;
		i++;
	}
	i = 0;
	while (i < sim.n_coders)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].sim = &sim;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % sim.n_coders];
		printf("Coders %d created\n", coders[i].id);
		printf("%d and %d are available to %d\n", coders[i].left_dongle->id,
			coders[i].right_dongle->id, coders[i].id);
		pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < sim.n_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}
}

