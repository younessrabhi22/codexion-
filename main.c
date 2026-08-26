/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 02:18:18 by yrabhi            #+#    #+#             */
/*   Updated: 2026/08/25 14:24:23 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char **argv)
{
    t_simulation *sim;

    if (argc != 9 || !init_rules(&(t_rules){0}, argv))
        return (1);
    sim = malloc(sizeof(t_simulation));
    if (!sim)
    return (printf("Error\n"), (1));

    printf("simulation malloc-ed ok\n");
    free(sim);
    return (0);
}
