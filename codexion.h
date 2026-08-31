#ifndef CODEXION_H
# define CODEXION_H

#include <sys/time.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


typedef struct s_rules
{
    int num_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int compiles_required;
    int dongle_cooldown;
    char *scheduler;
} t_rules;


typedef struct s_coder
{
    int id;
    struct s_dongle *left_dongle;
    struct s_dongle *right_dongle;
    pthread_t thread;
    int compile_count;
    long last_compile_start_ms;
    struct s_simulation *sim;
} t_coder;


typedef struct s_request
{
    t_coder *coder;
    long priority;
} t_request;


typedef struct s_queue
{
    t_request requests[2];
    int size;
} t_queue;


typedef struct s_dongle
{
    int id;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int is_free;
    long free_since_ms;
    t_queue waiters;
} t_dongle;


typedef struct s_simulation
{
    t_rules rules;
    t_coder *coders;
    t_dongle *dongles;
    pthread_mutex_t log_mutex;
    pthread_mutex_t stop_mutex;
    int stop;
    long start_time_ms;
} t_simulation;



int     init_rules(t_rules *rules ,char **argv);
int     init_dongles(t_simulation *sim);
int     init_coders(t_simulation *sim);
long    get_now_ms(void);
int take_dongle(t_coder *coder, t_dongle *dongle);
void release_dongle(t_dongle *dongle);

#endif
