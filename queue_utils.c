/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:28:58 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/31 12:12:29 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	scheduler_enqueue(t_dongle *d, t_coders *coder)
{
	long long	deadline_ms;

	pthread_mutex_lock(&coder->state_mutex);
	deadline_ms = coder->last_compile + coder->sim->time_to_burnout_ms;
	pthread_mutex_unlock(&coder->state_mutex);
	if (coder->sim->scheduler == 1)
		prepend_edf(&d->queue, coder->id, deadline_ms);
	else
		append_fifo(&d->queue, coder->id, deadline_ms);
}

int	is_before(long long deadline_ms, int coder_id, t_queue_node *node)
{
	return (deadline_ms < node->deadline_ms
		|| (deadline_ms == node->deadline_ms && coder_id < node->coder_id));
}
