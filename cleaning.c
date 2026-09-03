/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 05:20:13 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 02:42:24 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "codexion.h"

void destroy_dongles(t_sim *sim, int i)
{
    while (i > 0)
    {
        i--;
        pthread_cond_destroy(&sim->dongles[i].cond_var);
        pthread_mutex_destroy(&sim->dongles[i].lock);
    }
    free(sim->dongles);
    sim->dongles = NULL;
}

void destroy_coders(t_sim *sim, int i)
{
    while (i > 0)
    {
        i--;
        pthread_cond_destroy(&sim->coders[i].local_cond);
    }
    free(sim->coders);
    sim->coders = NULL;
}
void cleanup_simulation(t_sim *sim, int threads_created)
{
    int i;

    (void)threads_created;
    i = 0;
    while (i < sim->rules.num_coders)
    {
        pthread_mutex_destroy(&sim->dongles[i].lock);
        pthread_cond_destroy(&sim->dongles[i].cond_var);
        pthread_cond_destroy(&sim->coders[i].local_cond);
        i++;
    }
    pthread_mutex_destroy(&sim->print_lock);
    pthread_mutex_destroy(&sim->state_lock);
    if (sim->coders)
        free(sim->coders);
    if (sim->dongles)
        free(sim->dongles);
}
