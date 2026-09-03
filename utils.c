/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 05:21:54 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 02:45:43 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

long get_now_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int check_simulation_status(t_sim *sim)
{
    int status;

    pthread_mutex_lock(&sim->state_lock);
    status = sim->is_running;
    pthread_mutex_unlock(&sim->state_lock);

    return (status);
}


void *handle_single_coder(t_coder *coder)
{
    t_sim *sim;

    sim = coder->hub;

    pthread_mutex_lock(&coder->dongle_left->lock);

    pthread_mutex_lock(&sim->print_lock);
    printf("%ld %d has taken a dongle\n", get_now_ms() - sim->start_ts, coder->id);
    pthread_mutex_unlock(&sim->print_lock);

    while (check_simulation_status(sim))
    {
        usleep(1000);
    }

    pthread_mutex_unlock(&coder->dongle_left->lock);
    return (NULL);
}


int start_simulation(t_sim *sim)
{
    int i;
    pthread_t monitor_thread;

    sim->start_ts = get_now_ms();

    i = 0;
    while (i < sim->rules.num_coders)
    {
        sim->coders[i].last_compile_ts = sim->start_ts;
        pthread_create(&sim->coders[i].thread_id, NULL, coder_routine, &sim->coders[i]);
        i++;
    }

    pthread_create(&monitor_thread, NULL, monitor_routine, sim);

    pthread_join(monitor_thread, NULL);

    i = 0;
    while (i < sim->rules.num_coders)
    {
        pthread_join(sim->coders[i].thread_id, NULL);
        i++;
    }
    return (1);
}
