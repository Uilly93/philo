/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:07:16 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/08 15:04:00 by wnocchi          ###   ########.fr       */
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

typedef struct s_philo	t_philo;

typedef struct s_infos
{
	int				nb;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	struct			timeval start;
	struct			timeval end;
	int				loop;
	pthread_mutex_t	print;

	t_philo			*philos;
}	t_infos;

typedef struct s_philo
{
	pthread_t		threads;
	pthread_mutex_t *r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*print;
	int				index;
	t_infos			*infos;
	bool			eaten;
	bool			slept;
	int				forks_count;
}	t_philo;


#endif