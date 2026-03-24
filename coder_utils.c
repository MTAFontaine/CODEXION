/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:37:53 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/20 12:52:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_now_in_ms(void)
{
	long long		ms_now;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ms_now = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms_now);
}

void	*coder_routine(void *arg)
{
	t_coders	*coder;
	int i;

	i = 0;
	if (!arg)
		return (NULL);
	coder = (t_coders *)arg;
	while (coder->compile_count < coder->sim->number_of_compiles_required)
	{
		compile(*coder);
		coder->compile_count += 1;
		debug(*coder);
		refactor(*coder);
		i++;
	}
	return (NULL);
}

void	compile(t_coders coder)
{
	long long	ms_now;
	long long	target;
	long long	t_compile;

	get_dongle(coder.left_dongle, coder.id);
	get_dongle(coder.right_dongle, coder.id);
	t_compile = coder.sim->time_to_compile_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_compile;

	printf("\n%i Started compiling...\n", coder.id);
	while (ms_now < target)
		ms_now = get_now_in_ms();
	printf("\n%d Compiled!\n", coder.id);
	release_dongle(coder.left_dongle);
	release_dongle(coder.right_dongle);
}

void	refactor(t_coders coder)
{
	long long	ms_now;
	long long	target;
	long long	t_refactor;

	t_refactor = coder.sim->time_to_refactor_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_refactor;

	printf("\n%d Started refactoring...\n", coder.id);
	while (ms_now < target)
		ms_now = get_now_in_ms();
	printf("\n%d refactored!\n", coder.id);
}

void	debug(t_coders coder)
{
	long long	ms_now;
	long long	target;
	long long	t_debug;

	t_debug = coder.sim->time_to_debug_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_debug;
	printf("\n%d Started debugging...\n", coder.id);
	while (ms_now < target)
		ms_now = get_now_in_ms();
	printf("\n%d debugged!\n", coder.id);
}
