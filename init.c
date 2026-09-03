/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 00:53:16 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 21:49:43 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_dongles(t_sim *sim)
{
    int i;

    sim->dongles = malloc(sizeof(t_dongle) * sim->rules.num_coders);
    if (!sim->dongles)
        return (0);
    i = 0;
    while (i < sim->rules.num_coders)
    {
        sim->dongles[i].is_free = 1;
        sim->dongles[i].ready_at_ms = sim->start_ts;
        sim->dongles[i].waitlist.count = 0;
        if (pthread_mutex_init(&sim->dongles[i].lock, NULL) != 0)
        {
            destroy_dongles(sim, i);
            return (0);
        }
        if (pthread_cond_init(&sim->dongles[i].cond_var, NULL) != 0)
        {
            pthread_mutex_destroy(&sim->dongles[i].lock);
            destroy_dongles(sim, i);
            return (0);
        }
        i++;
    }
    return (1);
}
int init_coders(t_sim *sim)
{
    int i;

    sim->coders = malloc(sizeof(t_coder) * sim->rules.num_coders);
    if (!sim->coders)
        return (0);
    i = 0;
    while (i < sim->rules.num_coders)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].dongle_left = &sim->dongles[i];
        sim->coders[i].dongle_right = &sim->dongles[(i + 1) % sim->rules.num_coders];
        if (i == sim->rules.num_coders - 1)
        {
            sim->coders[i].dongle_left = &sim->dongles[(i + 1) % sim->rules.num_coders];
            sim->coders[i].dongle_right = &sim->dongles[i];
        }
        sim->coders[i].compile_count = 0;
        sim->coders[i].last_compile_ts = sim->start_ts;
        sim->coders[i].hub = sim;
        if (pthread_cond_init(&sim->coders[i].local_cond, NULL) != 0)
        {
            destroy_coders(sim, i);
            return (0);
        }
        i++;
    }
    return (1);
}

int init_simulation(t_sim *sim, t_rules *rules)
{
    struct timeval tv;

    sim->rules = *rules;
    sim->is_running = 1;

    sim->monitor_created = 0;

    if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&sim->state_lock, NULL) != 0)
        return (0);

    gettimeofday(&tv, NULL);
    sim->start_ts = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);


    if (!init_dongles(sim))
        return (0);

    if (!init_coders(sim))
        return (0);

    return (1);
}
