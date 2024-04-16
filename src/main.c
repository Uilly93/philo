/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:41:40 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/16 11:12:40 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(int nb_thread, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < nb_thread)
		pthread_join(philo[i++].threads, NULL);
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
