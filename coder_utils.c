/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:37:53 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/27 11:02:31 by marvin           ###   ########.fr       */
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
	int			i;

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
	t_dongle		*first;
	t_dongle		*second;
	long long		ms_now;
	long long		target;
	long long		t_compile;

	first = coder.left_dongle;
	second = coder.right_dongle;
	if (first->id > second->id)
	{
		first = coder.right_dongle;
		second = coder.left_dongle;
	}
	get_dongle(first, coder);
	get_dongle(second, coder);
	t_compile = coder.sim->time_to_compile_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_compile;

	coder.last_compile  = get_now_in_ms();
	pthread_mutex_lock(&coder.sim->output_mutex);
	printf("%lld %i is compiling\n",
		(get_now_in_ms() - coder.sim->start), coder.id);
	pthread_mutex_unlock(&coder.sim->output_mutex);
	while (ms_now < target)
		ms_now = get_now_in_ms();
	release_dongle(second);
	release_dongle(first);
}

void	refactor(t_coders coder)
{
	long long	ms_now;
	long long	target;
	long long	t_refactor;

	t_refactor = coder.sim->time_to_refactor_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_refactor;

	pthread_mutex_lock(&coder.sim->output_mutex);
	printf("%lld %d is refactoring\n",
		(get_now_in_ms() - coder.sim->start), coder.id);
	pthread_mutex_unlock(&coder.sim->output_mutex);
	while (ms_now < target)
		ms_now = get_now_in_ms();
}

void	debug(t_coders coder)
{
	long long	ms_now;
	long long	target;
	long long	t_debug;

	t_debug = coder.sim->time_to_debug_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_debug;
	pthread_mutex_lock(&coder.sim->output_mutex);
	printf("%lld %d is debugging\n",
		(get_now_in_ms() - coder.sim->start), coder.id);
	pthread_mutex_unlock(&coder.sim->output_mutex);
	while (ms_now < target)
		ms_now = get_now_in_ms();
}
