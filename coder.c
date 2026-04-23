/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:37:53 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/14 08:28:31 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	sleep_interruptible(t_sim *sim, long long duration_ms)
{
	long long	start;
	long long	now;

	start = get_now_in_ms();
	while (!is_sim_stopped(sim))
	{
		now = get_now_in_ms();
		if (now - start >= duration_ms)
			break ;
		usleep(500);
	}
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
		compile(coder);
		if (is_sim_stopped(coder->sim))
			break ;
		pthread_mutex_lock(&coder->state_mutex);
		coder->compile_count += 1;
		pthread_mutex_unlock(&coder->state_mutex);
		debug(coder);
		if (is_sim_stopped(coder->sim))
			break ;
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
	t_compile = coder->sim->time_to_compile_ms;
	if (!acquire_dongles(coder))
		return ;

	// Lock state_mutex before modifying last_compile
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile = get_now_in_ms();
	pthread_mutex_unlock(&coder->state_mutex);

	pthread_mutex_lock(&coder->sim->output_mutex);
	printf("%lld %i is compiling\n",
		(get_now_in_ms() - coder->sim->start), coder->id);
	pthread_mutex_unlock(&coder->sim->output_mutex);

	sleep_interruptible(coder->sim, t_compile);
	release_dongle(second);
	release_dongle(first);
}

void	refactor(t_coders *coder)
{
	long long	t_refactor;

	if (is_sim_stopped(coder->sim))
		return ;
	t_refactor = coder->sim->time_to_refactor_ms;
	pthread_mutex_lock(&coder->sim->output_mutex);
	printf("%lld %d is refactoring\n",
		(get_now_in_ms() - coder->sim->start), coder->id);
	pthread_mutex_unlock(&coder->sim->output_mutex);
	sleep_interruptible(coder->sim, t_refactor);
}

void	debug(t_coders *coder)
{
	long long	t_debug;

	if (is_sim_stopped(coder->sim))
		return ;
	t_debug = coder->sim->time_to_debug_ms;
	pthread_mutex_lock(&coder->sim->output_mutex);
	printf("%lld %d is debugging\n",
		(get_now_in_ms() - coder->sim->start), coder->id);
	pthread_mutex_unlock(&coder->sim->output_mutex);
	sleep_interruptible(coder->sim, t_debug);
}
