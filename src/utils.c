/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:29:38 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/16 10:31:24 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *s)
{
	int		i;
	int		sign;
	long	nbr;
	
	i = 0;
	sign = 1;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign *= -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		nbr = (nbr * 10 + s[i] - '0');
		i++;
	}
	return (nbr * sign);
}

long	get_end(t_philo *philo)
{
	long	start;
	long	end;
	
	gettimeofday(&philo->infos->end, NULL);
	start = ((philo->infos->start.tv_sec * 1000000)
	+ philo->infos->start.tv_usec) / 1000;
	end = ((philo->infos->end.tv_sec * 1000000)
	+ philo->infos->end.tv_usec) / 1000;
	return (end - start);
}

int	stop_simulation(t_philo *philo)
{
	pthread_mutex_lock(&philo->infos->mutex);
	if(philo->infos->dead || philo->infos->finished)
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->infos->mutex);
	return(0);
}

void	set_as_finished(t_philo *philo)
{
	int i;
	
	i = 0;
	while (i < philo->infos->nb)
	{
		pthread_mutex_lock(&philo->infos->mutex);
		philo[i].infos->finished = true;
		pthread_mutex_unlock(&philo->infos->mutex);
		i++;
	}
}

void	free_mutexs(t_philo *philo)
{
	int i;

	i = 0;
	while(i < philo->infos->nb)
	{
		if(philo[i].r_fork)
			free(philo[i].r_fork);
		i++;
	}
	free(philo);
	return ;
}
