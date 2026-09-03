/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:52:45 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 21:39:59 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void do_debug(t_coder *coder)
{
    if (!print_status(coder->hub, coder->id, "is debugging"))
        return;
    smart_sleep(coder->hub, coder->hub->rules.time_debug);
}

void do_refactor(t_coder *coder)
{
    if (!print_status(coder->hub, coder->id, "is refactoring"))
        return;
    smart_sleep(coder->hub, coder->hub->rules.time_refactor);
}
