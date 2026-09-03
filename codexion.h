/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 21:36:36 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 21:47:22 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>

#define FIFO_SCHED 0
#define EDF_SCHED 1

typedef struct s_sim t_sim;
typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;

typedef struct s_rules
{
    int num_coders;
    long time_burnout;
    long time_compile;
    long time_debug;
    long time_refactor;
    int req_compiles;
    long cooldown_ms;
    int sched_policy;
} t_rules;

typedef struct s_wait_node
{
    t_coder *coder;
    long priority_val;
} t_wait_node;

typedef struct s_waitlist
{
    t_wait_node list[2];
    int count;
} t_waitlist;

typedef struct s_dongle
{
    pthread_mutex_t lock;
    pthread_cond_t cond_var;
    int is_free;
    long ready_at_ms;
    t_waitlist waitlist;
} t_dongle;

typedef struct s_coder
{
    int id;
    pthread_t thread_id;
    pthread_cond_t local_cond;
    long last_compile_ts;
    int compile_count;
    t_dongle *dongle_left;
    t_dongle *dongle_right;
    t_sim *hub;
} t_coder;

struct s_sim
{
    t_rules rules;
    long start_ts;
    int is_running;
    pthread_t monitor_id;
    pthread_mutex_t state_lock;
    pthread_mutex_t print_lock;
    t_coder *coders;
    t_dongle *dongles;
    int monitor_created;
    pthread_t monitor_thread_id;
};

/* parser.c */
int init_rules(t_rules *rules, char **argv);

/* init.c */
int init_dongles(t_sim *sim);
int init_coders(t_sim *sim);
int init_simulation(t_sim *sim, t_rules *rules);

/* cleaning.c */
void destroy_dongles(t_sim *sim, int i);
void destroy_coders(t_sim *sim, int i);
void cleanup_simulation(t_sim *sim, int threads_created);

/* utils.c */
long get_now_ms(void);
int check_simulation_status(t_sim *sim);
void *handle_single_coder(t_coder *coder);

/* routine.c */
void *coder_routine(void *arg);
void smart_sleep(t_sim *sim, long time_to_sleep_ms);
void release_dongle(t_dongle *dongle, t_sim *sim);
void release_dongles(t_coder *coder);

/* routine_utils.c */
int get_dongle(t_coder *coder, t_dongle *dongle);
int print_status(t_sim *sim, int id, char *msg);
int take_dongles(t_coder *coder);

/* routine_utils2.c */
void do_debug(t_coder *coder);
void do_refactor(t_coder *coder);

/* scheduler.c */
long get_priority(t_coder *coder);

/* queue.c */
void add_to_queue(t_waitlist *q, t_coder *coder, long priority);
void remove_from_queue(t_waitlist *q);
void remove_specific_from_queue(t_waitlist *q, int coder_id);

/* monitor.c */
void *monitor_routine(void *arg);

#endif
