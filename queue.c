/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:23:42 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 21:39:08 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void add_to_queue(t_waitlist *q, t_coder *coder, long priority)
{
    if (q->count < 2)
    {
        q->list[q->count].coder = coder;
        q->list[q->count].priority_val = priority;
        q->count++;
        if (q->count == 2 && q->list[1].priority_val < q->list[0].priority_val)
        {
            t_wait_node tmp;

            tmp = q->list[0];
            q->list[0] = q->list[1];
            q->list[1] = tmp;
        }
    }
}

void remove_from_queue(t_waitlist *q)
{
    if (q->count > 0)
    {
        q->list[0] = q->list[1];
        q->count--;
    }
}

void remove_specific_from_queue(t_waitlist *q, int coder_id)
{
    if (q->count > 0 && q->list[0].coder->id == coder_id)
    {
        q->list[0] = q->list[1];
        q->count--;
    }
    else if (q->count > 1 && q->list[1].coder->id == coder_id)
        q->count--;
}
