/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:33:05 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/16 11:09:51 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_loop_eat(t_philo *philo)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < philo->infos->nb)
	{
		pthread_mutex_lock(&philo->infos->mutex);
		if (philo[i].eat_count == philo->infos->nb_loop)
			count++;
		pthread_mutex_unlock(&philo->infos->mutex);
		i++;
	}
	if (count == philo->infos->nb)
	{
		set_as_finished(philo);
		join_threads(philo->infos->nb, philo);
		pthread_mutex_lock(&philo->infos->mutex);
		if (philo->infos->loop)
			printf("All philosophers ate %d times, end of the simulation\n",
				philo->infos->nb_loop);
		pthread_mutex_unlock(&philo->infos->mutex);
		return (1);
	}
	return (0);
}

int	check_dead(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->infos->nb)
	{
		if (philo[i].infos->dead)
			return (1);
		i++;
	}
	return (0);
}

int	check_end(t_philo *philo)
{
	while (1)
	{
		if (philo->infos->loop)
		{
			if (check_loop_eat(philo))
				return (1);
		}
		if (set_as_dead(philo))
			return (1);
		usleep(1000);
	}
	return (0);
}

int	who_died(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->infos->nb)
	{
		if (philo[i].infos->dead)
			return (i + 1);
	}
	return (0);
}

int	set_as_dead(t_philo *philo)
{
	int		i;
	long	eat_limit;

	i = 0;
	eat_limit = 0;
	while (i < philo->infos->nb)
	{
		pthread_mutex_lock(&philo->infos->mutex);
		eat_limit = get_end(philo) - philo[i].last_meal;
		pthread_mutex_unlock(&philo->infos->mutex);
		if (eat_limit > philo->infos->die_time || check_dead(philo))
		{
			set_as_finished(philo);
			pthread_mutex_lock(&philo->infos->mutex);
			philo[i].infos->dead = true;
			printf("%ld philo %d died\n", get_end(philo), who_died(philo));
			pthread_mutex_unlock(&philo->infos->mutex);
			join_threads(philo->infos->nb, philo);
			return (1);
		}
		i++;
	}
	return (0);
}
