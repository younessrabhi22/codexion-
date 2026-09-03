/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrabhi <yrabhi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 02:18:29 by yrabhi            #+#    #+#             */
/*   Updated: 2026/09/03 02:31:04 by yrabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int	is_valid_integer(char **str)
{
	int	i;
    int j;

    j = 1;
    while (j < 8)
    {
        i = 0;
        if (!str[j] || str[j][0] == '\0')
            return (0);
        if (str[j][i] == '+')
            i++;
        if (str[j][i] == '\0')
            return (0);
        while (str[j][i])
        {
            if (str[j][i] < '0' || str[j][i] > '9')
                return (0);
            i++;
        }
        j++;
    }
	return (1);
}
long	ft_atoi(char *str)
{
	long	i;
	long	result;

	i = 0;
	result = 0;

    if (str[i] == '+')
    i++;

	while (str[i])
	{
		result = (result * 10) + (str[i] - '0');
		if ((result > 2147483647))
			return 2147483648;
		i++;
	}
	return (result);
}

int    init_rules(t_rules *rules, char **argv)
{
    int values[7];
    long tmp;
    int i;

    tmp = 0;
    i = 1;

    if (!is_valid_integer(argv))
        return (0);
    while (i <=7)
    {
        tmp = ft_atoi(argv[i]);
        if (tmp < 0 || tmp > INT_MAX)
            return (0);
        values[i - 1] = (int)tmp;
        i++;
    }
    if (values[0] == 0)
		return (0);
    rules->num_coders = values[0];
    rules->time_burnout = values[1];
    rules->time_compile = values[2];
    rules->time_debug = values[3];
    rules->time_refactor = values[4];
    rules->req_compiles = values[5];
    rules->cooldown_ms = values[6];

    if (strcmp(argv[8], "fifo") == 0)
        rules->sched_policy = FIFO_SCHED;
    else if (strcmp(argv[8], "edf") == 0)
        rules->sched_policy = EDF_SCHED;
    else
    {
        printf("Error: Scheduler must be exactly 'fifo' or 'edf'\n");
        return (0);
    }

    return 1;
}
