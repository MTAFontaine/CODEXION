/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:45 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/14 08:12:09 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	append_fifo(t_queue	*queue, int coder_id, long long deadline_ms)
{
	t_queue_node	*new_node;

	new_node = malloc(sizeof(t_queue_node));
	if (!new_node)
		exit(-1);
	new_node->coder_id = coder_id;
	new_node->deadline_ms = deadline_ms;
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

void	prepend_edf(t_queue *queue, int coder_id, long long deadline_ms)
{
	t_queue_node	*new_node;
	t_queue_node	*cur;

	new_node = malloc(sizeof(t_queue_node));
	if (!new_node)
		exit(-1);
	new_node->coder_id = coder_id;
	new_node->deadline_ms = deadline_ms;
	new_node->next = NULL;
	if (!queue->head || is_before(deadline_ms, coder_id, queue->head))
	{
		if (!queue->head)
			queue->tail = new_node;
		else
			new_node->next = queue->head;
		queue->head = new_node;
		return ;
	}
	cur = queue->head;
	while (cur->next && !is_before(deadline_ms, coder_id, cur->next))
		cur = cur->next;
	new_node->next = cur->next;
	cur->next = new_node;
	if (new_node->next == NULL)
		queue->tail = new_node;
}

int	peek(t_queue	*queue)
{
	if (queue->head == NULL)
		return (-1);
	return (queue->head->coder_id);
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
