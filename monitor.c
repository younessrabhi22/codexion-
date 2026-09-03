/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 05:20:54 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 02:09:33 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void wake_all_coders(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->rules.num_coders)
    {
        pthread_mutex_lock(&sim->dongles[i].lock);
        pthread_cond_broadcast(&sim->dongles[i].cond_var);
        pthread_mutex_unlock(&sim->dongles[i].lock);
        i++;
    }
}

static int check_burnout(t_sim *sim)
{
    long time_passed;
    int i;

    i = 0;
    while (i < sim->rules.num_coders)
    {
        pthread_mutex_lock(&sim->state_lock);
        time_passed = get_now_ms() - sim->coders[i].last_compile_ts;
        if (time_passed >= sim->rules.time_burnout)
        {
            sim->is_running = 0;
            pthread_mutex_unlock(&sim->state_lock);

            print_status(sim, sim->coders[i].id, "burned out");
            wake_all_coders(sim);
            return (1);
        }
        pthread_mutex_unlock(&sim->state_lock);
        i++;
    }
    return (0);
}

static int check_compiles(t_sim *sim)
{
    int i;
    int compiles;

    if (sim->rules.req_compiles == -1)
        return (0);
    i = 0;
    compiles = 0;
    while (i < sim->rules.num_coders)
    {
        pthread_mutex_lock(&sim->state_lock);
        if (sim->coders[i].compile_count >= sim->rules.req_compiles)
            compiles++;
        pthread_mutex_unlock(&sim->state_lock);
        i++;
    }
    if (compiles == sim->rules.num_coders)
    {
        pthread_mutex_lock(&sim->state_lock);
        sim->is_running = 0;
        pthread_mutex_unlock(&sim->state_lock);
        wake_all_coders(sim);
        return (1);
    }
    return (0);
}

void *monitor_routine(void *arg)
{
    t_sim *sim;

    sim = (t_sim *)arg;
    while (check_simulation_status(sim))
    {
        usleep(500);
        if (check_burnout(sim))
            return (NULL);
        if (check_compiles(sim))
            return (NULL);
    }
    return (NULL);
}
