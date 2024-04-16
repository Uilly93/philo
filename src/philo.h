/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:01:50 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/16 11:17:22 by wnocchi          ###   ########.fr       */
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
	struct timeval	start;
	struct timeval	end;
	int				nb_loop;
	bool			finished;
	bool			loop;
	bool			dead;
	pthread_mutex_t	mutex;
}	t_infos;

typedef struct s_philo
{
	pthread_t		threads;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	int				index;
	int				eat_count;
	long			last_meal;
	t_infos			*infos;
}	t_philo;

void	create(int nb_thread, t_philo *philo);
int		init_mutex(t_philo *philo);
void	give_forks(t_philo *philo);
void	lock_forks(t_philo *philo);
void	unlock_forks(t_philo *philo);

int		ft_atoi(char *s);
long	get_end(t_philo *philo);
int		stop_simulation(t_philo *philo);
void	set_as_finished(t_philo *philo);
void	free_mutexs(t_philo *philo);

int		check_loop_eat(t_philo *philo);
int		check_dead(t_philo *philo);
int		check_end(t_philo *philo);
int		who_died(t_philo *philo);
int		set_as_dead(t_philo *philo);

int		edge_case(t_philo *philo);
void	*routine(void *args);
void	eat_routine(t_philo *philo);
void	think_sleep(t_philo *philo);

bool	only_digit_string(char *s);
int		check_error(t_infos *infos);
bool	overflow(char *s);
int		check_overflow(t_infos *infos, char **av);
int		parsing_infos(t_philo **philo, t_infos *infos, char **av);

void	join_threads(int nb_thread, t_philo *philo);

#endif