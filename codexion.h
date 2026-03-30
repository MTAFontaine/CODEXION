/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:30:52 by mafontai          #+#    #+#             */
/*   Updated: 2026/03/30 15:58:31 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

typedef struct queue_node_s
{
	int						coder_id;
	long long				deadline_ms;
	struct queue_node_s		*next;
}	t_queue_node;

typedef struct queue_s
{
	t_queue_node	*head; // The front of the queue (the first one to ask)
	t_queue_node	*tail; // The back of the queue (the last one to ask)
}	t_queue;

typedef struct sim_s
{
	pthread_mutex_t	output_mutex;
	pthread_mutex_t	stop_mutex;
	int				n_coders;
	long long		start;
	int				time_to_burnout_ms;
	int				time_to_compile_ms;
	int				time_to_debug_ms;
	int				time_to_refactor_ms;
	int				number_of_compiles_required;
	int				dongle_cooldown_ms;
	int				scheduler;
	int				stop_flag;

}	t_sim;

typedef struct dongle_s
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				id;
	int				used;
	int				cooldown_time;
	long long		available_time;
	t_queue			queue;
}	t_dongle;

typedef struct coders_s
{
	int				id;
	pthread_t		thread;
	int				compile_count;
	long long		last_compile;
	t_sim			*sim;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}	t_coders;

void		compile(t_coders coder);
void		debug(t_coders coder);
void		refactor(t_coders coder);
void		*coder_routine(void *arg);
long long	get_now_in_ms(void);

void		append_fifo(t_queue	*queue, int coder_id, long long deadline_ms);
void		prepend_edf(t_queue *queue, int coder_id, long long deadline_ms);
void		scheduler_enqueue(t_dongle *d, t_coders coder);
void		pop_head(t_queue	*queue);
int			peek(t_queue	*queue);
void		get_dongle(t_dongle *d, t_coders coder);
void		release_dongle(t_dongle *d);

void		init_dongles(t_dongle *dongles, t_sim *sim);
void		init_coders(t_sim *sim, t_dongle *dongles, t_coders *coders);

void		scheduler_enqueue(t_dongle *d, t_coders coder);


#endif
