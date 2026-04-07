/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafontai <mafontai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:30:52 by mafontai          #+#    #+#             */
/*   Updated: 2026/04/07 08:30:09 by mafontai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <time.h>
# include <stdlib.h>
# include <unistd.h>
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
	pthread_t		thread;
	pthread_mutex_t	state_mutex;
	int				id;
	int				compile_count;
	long long		last_compile;
	t_sim			*sim;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}	t_coders;

typedef struct monitor_s
{
	int			coders_finished;
	t_dongle	*dongles;
	t_coders	*coders;
	t_sim		*sim;
}	t_monitor;

void		compile(t_coders *coder);
void		debug(t_coders *coder);
void		refactor(t_coders *coder);
void		*coder_routine(void *arg);
long long	get_now_in_ms(void);

void		append_fifo(t_queue	*queue, int coder_id, long long deadline_ms);
void		prepend_edf(t_queue *queue, int coder_id, long long deadline_ms);
void		scheduler_enqueue(t_dongle *d, t_coders *coder);
void		pop_head(t_queue	*queue);
int			peek(t_queue	*queue);
void		get_dongle(t_dongle *d, t_coders *coder);
void		release_dongle(t_dongle *d);
void		acquire_dongles(t_coders *coder);

void		init_dongles(t_dongle *dongles, t_sim *sim);
void		init_coders(t_sim *sim, t_dongle *dongles, t_coders *coders);

int			is_before(long long deadline_ms, int coder_id, t_queue_node *node);
void		set_stop_flag(t_sim *sim);
int			is_sim_stopped(t_sim *sim);
void		*monitor_routine(void *arg);
void		broadcast_all_dongles(t_dongle *dongles, int n_coders);

#endif
