/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 02:18:18 by yrabhi            #+#    #+#             */
/*   Updated: 2026/08/31 01:31:03 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static int init_sim_state(t_simulation *sim)
{
    struct timeval tv;

    sim->stop = 0;
    // sim->scheduler = NULL;
    if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
        return (0);
    gettimeofday(&tv, NULL);
    sim->start_time_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (1);
}

static int init_simulation(t_simulation *sim, char **argv)
{
    if (!init_rules(&sim->rules, argv))
        return (0);
    if (!init_dongles(sim))
        return (0);
    if (!init_coders(sim))
        return (0);
    if (!init_sim_state(sim))
        return (0);
    return (1);
}

int main(int argc, char **argv)
{
    t_simulation *sim;

    if (argc != 9)
        return (printf("Error\n"), 1);
    sim = malloc(sizeof(t_simulation));
    if (!sim)
        return (printf("Error\n"), 1);
    if (!init_simulation(sim, argv))
    {
        free(sim);
        return (printf("Error\n"), 1);
    }
    printf("init ok, num_coders=%d, start_time=%ld, stop=%d\n",
           sim->rules.num_coders, sim->start_time_ms, sim->stop);
    free(sim->dongles);
    free(sim->coders);
    free(sim);
    return (0);
}
