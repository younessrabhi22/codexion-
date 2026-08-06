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
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;

	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		if ((sign == 1 && result > 2147483647) || (sign == -1 && result > 2147483648))
			return 2147483648;
		i++;
	}
	return (result * sign);
}

int    init_rules(t_rules *rules, char **argv)
{
    int values[7];
    long tmp;
    tmp = 0;
    int i = 0;

    if (!is_valid_integer(argv))
        return (0);
    while (i < 7)
    {
        tmp = ft_atoi(argv[i + 1]);
        if (tmp < 0 || tmp > INT_MAX)
            return (0);
        values[i] = (int)tmp;
        i++;
    }
    if (values[0] <= 0) /*the coders value should be positive*/
		return (0);
    rules->num_coders = values[0];
    rules->time_to_burnout = values[1];
    rules->time_to_compile = values[2];
    rules->time_to_debug = values[3];
    rules->time_to_refactor = values[4];
    rules->compiles_required = values[5];
    rules->dongle_cooldown = values[6];

    if (strcmp(argv[8], "fifo") == 0)
        rules->scheduler = "fifo";
    else if (strcmp(argv[8], "edf") == 0)
        rules->scheduler = "edf";
    else
        return 0;

    return 1;
}
