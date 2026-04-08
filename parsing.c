/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 11:25:53 by marvin            #+#    #+#             */
/*   Updated: 2026/04/08 11:25:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_and_populate(const char *s, int *out)
{
	long long	value;
	int			i;

	i = 0;
	value = 0;
	if (!s || !s[0])
		return (1);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
		value = value * 10 + (s[i] - '0');
		if (value > 2147483647)
			return (1);
		i++;
	}
	if (value <= 0)
		return (1);
	*out = (int)value;
	return (0);
}

int	validate_arguments(int argc, char *argv[], t_sim *sim)
{
	if (!sim || argc != 9)
		return (1);
	if (parse_and_populate(argv[1], &sim->n_coders)
		|| parse_and_populate(argv[2], &sim->time_to_burnout_ms)
		|| parse_and_populate(argv[3], &sim->time_to_compile_ms)
		|| parse_and_populate(argv[4], &sim->time_to_debug_ms)
		|| parse_and_populate(argv[5], &sim->time_to_refactor_ms)
		|| parse_and_populate(argv[6], &sim->number_of_compiles_required)
		|| parse_and_populate(argv[7], &sim->dongle_cooldown_ms))
		return (1);
	if (strcmp(argv[8], "FIFO") == 0)
		sim->scheduler = 0;
	else if (strcmp(argv[8], "EDF") == 0)
		sim->scheduler = 1;
	else
		return (1);
	sim->stop_flag = 0;
	pthread_mutex_init(&sim->output_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	return (0);
}
