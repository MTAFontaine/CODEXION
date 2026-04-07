/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 13:07:02 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/06 07:58:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "codexion.h"

void init_monitor(t_monitor ctx, t_dongle *dongles, t_coders *coders, t_sim *sim)
{
	ctx.coders_finished = 0;
	ctx.dongles = dongles;
	ctx.coders = coders;
	ctx.sim = sim;
}

void	init_dongles(t_dongle *dongles, t_sim *sim)
{
	int	i;

	pthread_mutex_init(&sim->output_mutex, NULL);
	i = 0;
	while (i < sim->n_coders)
	{
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
		dongles[i].id = i;
		dongles[i].used = 0;
		dongles[i].cooldown_time = sim->dongle_cooldown_ms;
		dongles[i].available_time = 0;
		dongles[i].queue.head = NULL;
		dongles[i].queue.tail = NULL;
		i++;
	}
}

void	init_coders(t_sim *sim, t_dongle *dongles, t_coders *coders)
{
	int	i;

	i = 0;
	while (i < sim->n_coders)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].sim = sim;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % sim->n_coders];
		coders[i].last_compile = sim->start;
		pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]);
		pthread_mutex_init(&coders[i].state_mutex, NULL);
		i++;
	}
}