/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 10:48:23 by marvin            #+#    #+#             */
/*   Updated: 2026/04/08 10:48:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_queues(t_dongle *all_dongles, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		clear_queue(&all_dongles[i].queue);
		i++;
	}
}

void	cleanup_mutexes(t_sim *sim, t_coders *coders, t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < sim->n_coders)
	{
		pthread_mutex_destroy(&coders[i].state_mutex);
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&sim->output_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
}

void	clear_queue(t_queue *queue)
{
	t_queue_node	*current;
	t_queue_node	*next;

	if (!queue)
		return ;
	current = queue->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	queue->head = NULL;
	queue->tail = NULL;
}
