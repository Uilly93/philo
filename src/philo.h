/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:07:16 by wnocchi           #+#    #+#             */
/*   Updated: 2024/03/14 13:41:47 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <bits/pthreadtypes.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>


typedef struct s_philo
{
	pthread_mutex_t mutex;
	int				philo;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	struct			timeval start;
	struct			timeval end;

	bool			eaten;
	bool			slept;
	int				forks_count;
}	t_philo;

typedef struct s_index
{
	int		i;
	// struct	t_philo;
}	t_index;

#endif