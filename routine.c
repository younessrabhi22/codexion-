/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 04:22:17 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 21:54:25 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void smart_sleep(t_sim *sim, long time_to_sleep_ms)
{
    long start_time;

    start_time = get_now_ms();
    while ((get_now_ms() - start_time) < time_to_sleep_ms && check_simulation_status(sim))
        usleep(500);
}

static int do_compile(t_coder *coder)
{
    t_sim *sim;

    sim = coder->hub;
    if (!print_status(sim, coder->id, "is compiling"))
        return (0);
    pthread_mutex_lock(&sim->state_lock);
    coder->last_compile_ts = get_now_ms();
    coder->compile_count++;
    pthread_mutex_unlock(&sim->state_lock);
    smart_sleep(sim, sim->rules.time_compile);
    return (1);
}

void release_dongle(t_dongle *dongle, t_sim *sim)
{
    pthread_mutex_lock(&dongle->lock);
    dongle->is_free = 1;
    dongle->ready_at_ms = get_now_ms() + sim->rules.cooldown_ms;
    pthread_cond_broadcast(&dongle->cond_var);
    pthread_mutex_unlock(&dongle->lock);
}

void release_dongles(t_coder *coder)
{
    release_dongle(coder->dongle_left, coder->hub);
    release_dongle(coder->dongle_right, coder->hub);
}

void *coder_routine(void *arg)
{
    t_coder *coder;
    t_sim *sim;

    coder = (t_coder *)arg;
    sim = coder->hub;
    if (sim->rules.num_coders == 1)
        return (handle_single_coder(coder));
    while (check_simulation_status(sim))
    {
        if (!take_dongles(coder))
            break;
        if (!do_compile(coder))
        {
            release_dongles(coder);
            break;
        }
        release_dongles(coder);
        if (!check_simulation_status(sim))
            break;
        do_debug(coder);
        if (!check_simulation_status(sim))
            break;
        do_refactor(coder);
    }
    return (NULL);
}
