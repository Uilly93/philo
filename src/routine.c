/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:35:34 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/16 11:01:06 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	edge_case(t_philo *philo)
{
	pthread_mutex_lock(&philo->infos->mutex);
	if(philo->infos->nb == 1)
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		usleep(philo->infos->dead * 1000);
		return (1);
	}
	pthread_mutex_unlock(&philo->infos->mutex);
	if (philo->index % 2 != 0)
		usleep(philo->infos->eat_time * 1000);
	if (philo->infos->nb % 2 != 0 && philo->index == philo->infos->nb - 1)
		usleep((philo->infos->sleep_time + philo->infos->eat_time) * 1000);
	return (0);
}

void	loop_routine(t_philo *philo)
{
	if(stop_simulation(philo))
		return ;
	while (1)
	{
		pthread_mutex_lock(&philo->infos->mutex);
		if(philo->infos->finished)
		{
			pthread_mutex_unlock(&philo->infos->mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->infos->mutex);
		eat_routine(philo);
	}
	return ;	
}

void	*routine(void *args)
{
	t_philo *philo;
	
	philo = (t_philo *)args;

	if(edge_case(philo))
		return(NULL);
	if(philo->infos->loop)
		loop_routine(philo);
	while (1)
	{
		if(stop_simulation(philo))
			return (NULL);
		eat_routine(philo);
	}
	return (NULL);
}

void	eat_routine(t_philo *philo)
{
	lock_forks(philo);
	pthread_mutex_lock(&philo->infos->mutex);
	if(philo->infos->finished) // reduce
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		unlock_forks(philo);
		return ;
	}
	printf("%ld %d is eating\n",get_end(philo), philo->index +1);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->infos->mutex);
	usleep(philo->infos->eat_time * 1000);
	pthread_mutex_lock(&philo->infos->mutex);
	philo->last_meal = get_end(philo);
	if(philo->infos->finished) // reduce
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		unlock_forks(philo);
		return ;
	}
	pthread_mutex_unlock(&philo->infos->mutex);
	unlock_forks(philo);
	think_sleep(philo);
}

void	think_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->infos->mutex);
	printf("%ld %d is sleeping\n",get_end(philo), philo->index +1);
	pthread_mutex_unlock(&philo->infos->mutex);
	usleep(philo->infos->sleep_time * 1000);
	pthread_mutex_lock(&philo->infos->mutex);
	if(philo->infos->finished) // reduce
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		return ;
	}
	printf("%ld %d is thinking\n",get_end(philo), philo->index +1);
	pthread_mutex_unlock(&philo->infos->mutex);
}
