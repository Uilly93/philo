/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:07:16 by wnocchi           #+#    #+#             */
/*   Updated: 2024/03/06 13:50:51 by wnocchi          ###   ########.fr       */
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
	int				index;
	int				forks;
	int				philo;
	long			time;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	struct			timeval tv;
	struct			timeval start;
	struct			timeval end;
	struct			timeval tmp;
}	t_philo;

#endif