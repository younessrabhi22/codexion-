/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 04:22:30 by yrabhi            #+#    #+#             */
/*   Updated: 2026/08/29 11:39:14 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void ms_to_timespec(long deadline_ms, struct timespec *ts)
{
    ts->tv_sec = deadline_ms / 1000;
    ts->tv_nsec = (deadline_ms % 1000) * 1000000;
}

int take_dongle(t_dongle *dongle, int cooldown_ms)
{
    long deadline;
    struct timespec ts;

    pthread_mutex_lock(&dongle->mutex);
    while (!dongle->is_free || get_now_ms() - dongle->free_since_ms < cooldown_ms)
    {
        if (!dongle->is_free)
            pthread_cond_wait(&dongle->cond, &dongle->mutex);
        else
        {
            deadline = dongle->free_since_ms + cooldown_ms;
            ms_to_timespec(deadline, &ts);
            pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
        }
    }
    dongle->is_free = 0;
    pthread_mutex_unlock(&dongle->mutex);
    return (1);
}

void release_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->is_free = 1;
    dongle->free_since_ms = get_now_ms();
    pthread_mutex_unlock(&dongle->mutex);
    pthread_cond_broadcast(&dongle->cond);
}

int	init_dongles(t_simulation *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->rules.num_coders);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->rules.num_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].is_free = 1;
		sim->dongles[i].free_since_ms = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
            return (0);
        if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
            return (0);
        i++;
	}
	return (1);
}
