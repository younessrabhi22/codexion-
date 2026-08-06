#ifndef CODEXION_H
# define CODEXION_H


#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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

int    init_rules(t_rules *rules ,char **argv);


#endif
