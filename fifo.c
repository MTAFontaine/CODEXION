/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:45 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/10 13:56:16 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	append(t_fifo_queue	*queue, int coder_id)
{
	t_queue_node	*new_node;

	new_node = malloc(sizeof(t_queue_node));
	if (!new_node)
		exit(-1);
	new_node->coder_id = coder_id;
	new_node->next = NULL;

	if (queue->tail == NULL)
	{
		queue->head = new_node;
		queue->tail = new_node;
	}
	else
	{
		queue->tail->next = new_node;
		queue->tail = new_node;
	}
}

int	peek(t_fifo_queue	*queue)
{
	if (queue->head == NULL)
		return (-1);
	return (queue->head->coder_id);
}

