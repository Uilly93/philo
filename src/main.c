/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:41:40 by wnocchi           #+#    #+#             */
/*   Updated: 2025/01/22 14:21:51 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long time, t_philo *philo)
{
	long	actual;

	pthread_mutex_lock(&philo->infos->mutex);
	actual = get_end(philo);
	pthread_mutex_unlock(&philo->infos->mutex);
	while (1)
	{
		pthread_mutex_lock(&philo->infos->mutex);
		if (get_end(philo) >= actual + time)
		{
			pthread_mutex_unlock(&philo->infos->mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->infos->mutex);
		usleep(50);
	}
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_infos	infos;

	if (!(ac == 5 || ac == 6))
		return (0);
	if (parsing_infos(&philo, &infos, av))
		return (1);
	gettimeofday(&infos.start, NULL);
	create(infos.nb, philo);
	check_end(philo);
	free_mutexs(philo);
	return (0);
}
