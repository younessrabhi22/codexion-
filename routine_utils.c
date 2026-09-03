/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 05:21:06 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 21:39:31 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int get_dongle(t_coder *coder, t_dongle *dongle)
{
    long priority;

    pthread_mutex_lock(&dongle->lock);
    if (!check_simulation_status(coder->hub))
    {
        pthread_mutex_unlock(&dongle->lock);
        return (0);
    }
    priority = get_priority(coder);
    add_to_queue(&dongle->waitlist, coder, priority);
    while (check_simulation_status(coder->hub) && (dongle->is_free == 0 || dongle->waitlist.list[0].coder->id != coder->id))
        pthread_cond_wait(&dongle->cond_var, &dongle->lock);
    if (!check_simulation_status(coder->hub))
    {
        remove_specific_from_queue(&dongle->waitlist, coder->id);
        pthread_mutex_unlock(&dongle->lock);
        return (0);
    }
    dongle->is_free = 0;
    remove_from_queue(&dongle->waitlist);
    pthread_mutex_unlock(&dongle->lock);
    while (check_simulation_status(coder->hub) && get_now_ms() < dongle->ready_at_ms)
        usleep(500);
    return (1);
}

int print_status(t_sim *sim, int id, char *msg)
{
    long time_now;

    pthread_mutex_lock(&sim->print_lock);
    if (sim->is_running == 0 && strcmp(msg, "burned out") != 0)
    {
        pthread_mutex_unlock(&sim->print_lock);
        return (0);
    }
    time_now = get_now_ms() - sim->start_ts;
    printf("%ld %d %s\n", time_now, id, msg);
    pthread_mutex_unlock(&sim->print_lock);
    return (1);
}

int take_dongles(t_coder *coder)
{
    if (!get_dongle(coder, coder->dongle_left))
        return (0);
    if (!print_status(coder->hub, coder->id, "has taken a dongle"))
        return (0);
    if (!get_dongle(coder, coder->dongle_right))
    {
        release_dongle(coder->dongle_left, coder->hub);
        return (0);
    }
    if (!print_status(coder->hub, coder->id, "has taken a dongle"))
    {
        release_dongle(coder->dongle_left, coder->hub);
        release_dongle(coder->dongle_right, coder->hub);
        return (0);
    }
    return (1);
}
