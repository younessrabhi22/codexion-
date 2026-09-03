/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 02:18:18 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 21:51:20 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int start_simulation(t_sim *sim)
{
    int i;
    pthread_t monitor_thread;

    sim->start_ts = get_now_ms();
    sim->is_running = 1;

    i = 0;
    while (i < sim->rules.num_coders)
    {
        sim->coders[i].last_compile_ts = sim->start_ts;
        if (pthread_create(&sim->coders[i].thread_id, NULL, coder_routine, &sim->coders[i]) != 0)
            return (0);
        i++;
    }
    if (pthread_create(&monitor_thread, NULL, monitor_routine, sim) != 0)
        return (0);
    pthread_join(monitor_thread, NULL);
    i = 0;
    while (i < sim->rules.num_coders)
    {
        pthread_join(sim->coders[i].thread_id, NULL);
        i++;
    }
    return (1);
}

int main(int argc, char **argv)
{
    t_rules rules;
    t_sim sim;

    if (argc != 9)
    {
        printf("Error: Invalid number of arguments\n");
        return (1);
    }
    if (!init_rules(&rules, argv))
        return (1);
    if (!init_simulation(&sim, &rules))
        return (1);
    if (!start_simulation(&sim))
    {
        cleanup_simulation(&sim, sim.rules.num_coders);
        return (1);
    }
    cleanup_simulation(&sim, sim.rules.num_coders);
    return (0);
}
