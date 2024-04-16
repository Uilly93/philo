/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:21:15 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/16 10:28:48 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create(int nb_thread, t_philo *philo)
{
	int i = 0;
		while(i < nb_thread)
		{
			pthread_create(&philo[i].threads, NULL, routine,(void *)&philo[i]);
			i++;
		}
		return ;
}

int	init_mutex(t_philo *philo)
{
	int i;

	i = 0;
	while(i < philo->infos->nb)
	{
		philo[i].r_fork = malloc(sizeof(pthread_mutex_t));
		if(!philo[i].r_fork)
		{
			free_mutexs(philo);
			return (1);
		}
		pthread_mutex_init(philo[i].r_fork, NULL);
		i++;
	}
	give_forks(philo);
	return (0);
}

void	give_forks(t_philo *philo)
{
	int i;

	i = 0;
	while (i < philo->infos->nb)
	{
		if (i == 0)
			philo[i].l_fork = philo[philo->infos->nb - 1].r_fork;
		else
			philo[i].l_fork = philo[i - 1].r_fork;
		i++;
	}
}

void	lock_forks(t_philo *philo)
{
	if(philo->r_fork && philo->r_fork)
	{
		if(philo->index % 2 == 0)
		{
			pthread_mutex_lock(philo->l_fork);
			pthread_mutex_lock(philo->r_fork);
		}
		else
		{
			pthread_mutex_lock(philo->r_fork);
			pthread_mutex_lock(philo->l_fork);
		}
	}
}

void	unlock_forks(t_philo *philo)
{
	if(philo->r_fork && philo->r_fork)
	{
		if(philo->index % 2 == 0)
		{
			pthread_mutex_unlock(philo->l_fork);
			pthread_mutex_unlock(philo->r_fork);
		}
		else
		{
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
		}
	}
}
