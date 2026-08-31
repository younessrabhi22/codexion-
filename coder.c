/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 04:22:17 by yrabhi            #+#    #+#             */
/*   Updated: 2026/08/29 08:35:13 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int init_coders(t_simulation *sim)
{
    int i;
    int n;

    n = sim->rules.num_coders;
    sim->coders = malloc(sizeof(t_coder) * n);
    if (!sim->coders)
        return (0);
    i = 0;
    while (i < n)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].left_dongle = &sim->dongles[i];
        sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
        sim->coders[i].compile_count = 0;
        sim->coders[i].last_compile_start_ms = sim->start_time_ms;
        sim->coders[i].sim = sim;
        i++;
    }
    return (1);
}
