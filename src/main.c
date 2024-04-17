/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:41:40 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/17 11:02:56 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
