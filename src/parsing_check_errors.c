/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check_errors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:39:06 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/16 11:15:22 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	only_digit_string(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
			i++;
		else
			return (true);
	}
	return (false);
}

int	check_error(t_infos *infos)
{
	if (infos->die_time < 0)
		return (1);
	if (infos->eat_time < 0)
		return (1);
	if (infos->sleep_time < 0)
		return (1);
	if (infos->nb < 0)
		return (1);
	return (0);
}

bool	overflow(char *s)
{
	int		i;
	int		sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ' || s[i] == '+')
		i++;
	if (s[i] == '-')
	{
		sign *= -1;
		i++;
	}
	if (only_digit_string(s + i))
		return (true);
	while (s[i] >= '0' && s[i] <= '9')
	{
		nbr = (nbr * 10 + s[i++] - '0');
		if (nbr * sign > INT_MAX || nbr * sign < INT_MIN)
			return (true);
	}
	return (false);
}

int	check_overflow(t_infos *infos, char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (overflow(av[i]))
		{
			write(2, "Error\nInvalid arguments\n", 24);
			return (1);
		}
		i++;
	}
	if (i == 6)
	{
		infos->loop = true;
		infos->nb_loop = ft_atoi(av[5]);
	}
	infos->finished = false;
	infos->nb = ft_atoi(av[1]);
	infos->die_time = ft_atoi(av[2]);
	infos->eat_time = ft_atoi(av[3]);
	infos->sleep_time = ft_atoi(av[4]);
	if (check_error(infos) || (infos->loop == true && infos->nb_loop <= 0))
		return (write(2, "Error\nInvalid arguments\n", 24), 1);
	return (0);
}

int	parsing_infos(t_philo **philo, t_infos *infos, char **av)
{
	int	i;

	i = 0;
	infos->loop = false;
	infos->dead = false;
	if (check_overflow(infos, av))
		return (1);
	*philo = malloc(sizeof(t_philo) * infos->nb);
	if (!*philo)
		return (1);
	pthread_mutex_init(&infos->mutex, NULL);
	while (i < infos->nb)
	{
		(*philo)[i].infos = infos;
		(*philo)[i].index = i;
		(*philo)[i].last_meal = 0;
		(*philo)[i].eat_count = 0;
		i++;
	}
	if (init_mutex(*philo))
		return (1);
	return (0);
}
