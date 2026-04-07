/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:37:53 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/07 08:31:32 by mafontai         ###   ########.fr       */
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

	if (!arg)
		return (NULL);
	coder = (t_coders *)arg;
	while (!is_sim_stopped(coder->sim)
		&& coder->compile_count < coder->sim->number_of_compiles_required)
	{
		if (is_sim_stopped(coder->sim))
			break;
		compile(coder);
		if (is_sim_stopped(coder->sim))
			break;
		pthread_mutex_lock(&coder->state_mutex);
		coder->compile_count += 1;
		pthread_mutex_unlock(&coder->state_mutex);
		if (is_sim_stopped(coder->sim))
			break;
		debug(coder);
		if (is_sim_stopped(coder->sim))
			break;
		refactor(coder);
	}
	return (NULL);
}

void	compile(t_coders *coder)
{
	t_dongle		*first;
	t_dongle		*second;
	long long		t_compile;

	first = coder->left_dongle;
	second = coder->right_dongle;
	acquire_dongles(coder);
	t_compile = coder->sim->time_to_compile_ms;
	coder->last_compile = get_now_in_ms();
	pthread_mutex_lock(&coder->sim->output_mutex);
	printf("%lld %i is compiling\n",
		(get_now_in_ms() - coder->sim->start), coder->id);
	pthread_mutex_unlock(&coder->sim->output_mutex);
	usleep(t_compile * 1000);
	release_dongle(second);
	release_dongle(first);
}

void	refactor(t_coders *coder)
{
	long long	ms_now;
	long long	target;
	long long	t_refactor;

	t_refactor = coder->sim->time_to_refactor_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_refactor;

	pthread_mutex_lock(&coder->sim->output_mutex);
	printf("%lld %d is refactoring\n",
		(get_now_in_ms() - coder->sim->start), coder->id);
	pthread_mutex_unlock(&coder->sim->output_mutex);
	if (!is_sim_stopped(coder->sim))
		usleep(t_refactor * 1000);
}

void	debug(t_coders *coder)
{
	long long	ms_now;
	long long	target;
	long long	t_debug;

	t_debug = coder->sim->time_to_debug_ms;
	ms_now = get_now_in_ms();
	target = ms_now + t_debug;
	pthread_mutex_lock(&coder->sim->output_mutex);
	printf("%lld %d is debugging\n",
		(get_now_in_ms() - coder->sim->start), coder->id);
	pthread_mutex_unlock(&coder->sim->output_mutex);
	if (!is_sim_stopped(coder->sim))
		usleep(t_debug * 1000);
}
