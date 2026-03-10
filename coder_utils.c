/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:37:53 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/10 13:59:10 by mafontai         ###   ########.fr       */
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
	long		t_compile;
	long		t_debug;
	long		t_refactor;

	if (!arg)
		return (NULL);
	coder = (t_coders *)arg;
	compile(*coder);
	debug(*coder);
	refactor(*coder);
	return (NULL);
}

void	compile(t_coders coder)
{
	long long	ms_now;
	long long	target;
	long long	t_compile;

	append(&coder.left_dongle->fifo, coder.id);
	append(&coder.right_dongle->fifo, coder.id);
	t_compile = coder.sim->time_to_compile_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_compile;

	printf("\n%lu Started compiling...\n", coder.id);
	while (ms_now < target)
		ms_now = get_now_in_ms();
	printf("\n%d Compiled!\n", coder.id);
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
