#ifndef CODEXION_H
# define CODEXION_H

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct s_rules
{
    int         num_coders;
    int         time_to_burnout;
    int         time_to_compile;
    int         time_to_debug;
    int         time_to_refactor;
    int         compiles_required;
    int         dongle_cooldown;
    char        *scheduler;
}   t_rules;


typedef struct s_dongle
{
    int id;
    pthread_mutex_t mutex;
    int is_free;
    long free_since_ms;
} t_dongle;


typedef struct s_coder
{
    int id;
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    pthread_t thread;
    int compile_count;
    long last_compile_start_ms;
    struct s_simulation *sim;
} t_coder;


typedef struct s_simulation
{
    t_rules rules;
    t_coder *coders;
    t_dongle *dongles;
    pthread_mutex_t log_mutex;
    pthread_mutex_t stop_mutex;
    int stop;
    long start_time_ms;
    void *scheduler;
} t_simulation;


typedef struct s_heap_node
{
    int coder_id;
    long priority_key;
} t_heap_node;


typedef struct s_heap
{
    t_heap_node *nodes;
    int size;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} t_heap;


int    init_rules(t_rules *rules ,char **argv);


#endif
