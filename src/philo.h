/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:07:16 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/11 19:12:42 by wnocchi          ###   ########.fr       */
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
	int				nb_loop;
	bool			finished;
	bool			loop;
	bool			dead;
	pthread_mutex_t	print;

	// t_philo			*philos;
}	t_infos;

typedef struct s_philo
{
	pthread_t		threads;
	pthread_mutex_t *r_fork;
	pthread_mutex_t	*l_fork;
	int				index;
	int				eat_count;
	long			last_meal;
	t_infos			*infos;
}	t_philo;


#endif