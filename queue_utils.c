/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:28:58 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/30 15:40:54 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	scheduler_enqueue(t_dongle *d, t_coders coder)
{
	long long	deadline_ms;

	deadline_ms = coder.last_compile + coder.sim->time_to_burnout_ms;
	if (coder.sim->scheduler == 1)
		prepend_edf(&d->queue, coder.id, deadline_ms);
	else
		append_fifo(&d->queue, coder.id, deadline_ms);
}

void	pop_head(t_queue *queue)
{
	t_queue_node	*old_head;

	if (!queue || !queue->head)
		return ;
	old_head = queue->head;
	queue->head = old_head->next;
	if (queue->head == NULL)
		queue->tail = NULL;
	free(old_head);
}

int	peek(t_queue	*queue)
{
	if (queue->head == NULL)
		return (-1);
	return (queue->head->coder_id);
}
