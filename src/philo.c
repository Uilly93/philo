/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:18:58 by wnocchi           #+#    #+#             */
/*   Updated: 2024/03/18 16:42:39 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

bool overflow(char *s)
{
	int		i;
	int		sign;
	long	nbr;
	
	i = 0;
	sign = 1;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if(s[i] == '-')
			sign *= -1;
		i++;
	}
	while(s[i] >= '0' && s[i] <= '9')
	{
		nbr *= 10;
		nbr += s[i] - 48;
		if(nbr * sign > INT_MAX || nbr * sign < INT_MIN)
			return (true);
		i++;
	}
	return (false);
}

int	ft_atoi(char *s)
{
	int		i;
	int		sign;
	long	nbr;
	
	i = 0;
	sign = 1;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if(s[i] == '-')
			sign *= -1;
		i++;
	}
	while(s[i] >= '0' && s[i] <= '9')
	{
		nbr *= 10;
		nbr += s[i] - 48;
		i++;
	}
	return (nbr * sign);
}

long	get_end(t_philo *philos)
{
	gettimeofday(&philos->end, NULL);
	// long time = (philos->end.tv_sec - philos->start.tv_sec);
	// long micros = ((time * 1000000) + philos->end.tv_usec) - (philos->start.tv_usec);
	// return (micros / 1000);
	long start = ((philos->start.tv_sec * 1000000) + philos->start.tv_usec) / 1000;
	long end = ((philos->end.tv_sec * 1000000) + philos->end.tv_usec) / 1000;
	return (end - start);
}
// void	print_end(t_philo *philos)
// {
// 	printf("%ld ms", get_end(philos));
// }

// long	get_start(t_philo *philos)
// {
// 	long	start;
// 	gettimeofday(&philos->start, NULL);
// 	return (start);
// }

int		eat_routine(t_philo *philos)
{
	// if(philos->forks_count == 2)
	// {
		usleep(philos->eat_time * 1000);
		// printf("%ld ms: ", get_end(philos));
		printf("philo %d eating\n", philos->index);
		return (0);
	// }
	// return (1);
}

int sleeping_routine(t_philo *philos)
{
	usleep(philos->sleep_time * 1000);
	// printf("%ld ms: ", get_end(philos));
	printf("philo %d sleeping\n", philos->index);
	// printf("%ldms philo %d sleeping\n", end, philos->philo);
	return (0);
}

int	thinking_routine(t_philo *philos)
{
	(void)philos;
	printf("philo %d thinking\n", philos->index);
	// printf("%ldms philo %d sleeping\n", end, philos->philo);
	return (0);
}

void	*thread_routine(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	// pthread_mutex_lock(&philos->mutex);
	// gettimeofday(&philos->start, NULL);
	// end = get_end(philos);
	// printf("%ld ms: ", get_end(philos));
	printf("%ld ms: philo %d take a fork\n", get_end(philos), philos->index);
	// pthread_mutex_unlock(&philos->mutex);
	if(eat_routine(philos))
		return (NULL);
	printf("%ld ms: ", get_end(philos));
	if(sleeping_routine(philos))
		return (NULL);
	printf("%ld ms: ", get_end(philos));
	return (NULL);
}

// void *print_thread(void *arg)
// {
// 	t_philo *philos = (t_philo *)arg;
// 	// usleep(200000);
// 	printf("this is philo %d\n", philos->philo);
// 	return (NULL);
// }

void	join_threads(int nb_thread, t_philo *philos, pthread_t *threads)
{
	int		i;
	(void)philos;
	
	i = 0;
	while(i < nb_thread){
		pthread_join(threads[i++], NULL);
	}
}

void	init_mutex(t_philo *philos)
{
	int i;
	
	i = 0;
	pthread_mutex_init(philos[i].right_fork, NULL);
	while(i < philos->nb)
	{
		philos[i].right_fork = malloc(sizeof(pthread_mutex_t));
		i++;
	}
	return ;
}
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	change_odd_forks(t_philo *philos, char *s)
{
	int i;

	i = 0;
	if(ft_strcmp(s, "lock") == 0)
	{
		while (i < philos->nb)
		{
			if(i % 2 != 0)
				pthread_mutex_lock(philos[i].right_fork);
			i++;
		}
	}
	else if (ft_strcmp(s, "unlock") == 0)
	{
		while (i < philos->nb)
		{
			if(i % 2 != 0)
				pthread_mutex_unlock(philos[i].right_fork);
			i++;
		}
	}
}

void	change_pair_forks(t_philo *philos, char *s)
{
	int i;

	i = 0;
	if(ft_strcmp(s, "lock") == 0)
	{
		while (i < philos->nb)
		{
			if(i % 2 == 0)
				pthread_mutex_lock(philos[i].right_fork);
			i++;
		}
	}
	else if (ft_strcmp(s, "unlock") == 0)
	{
		while (i < philos->nb)
		{
			if(i % 2 == 0)
				pthread_mutex_unlock(philos[i].right_fork);
			i++;
		}
	}
}

void	*pair_routine(void *args)
{
	t_philo *philos;
	int	i;
	
	philos = (t_philo *)args;
	i = 0;
	while (i < philos->nb)
	{
		philos[i].forks_count = 1;
		philos[i].right_fork = malloc(sizeof(pthread_mutex_t));
		i++;
	}
	i = 0;
	while (i < philos->nb)
	{
		// change_odd_forks(philos, "lock");
		if(i % 2 == 0)
		{
			if(i == 0)
				philos[0].left_fork = philos[philos->nb].right_fork;
			philos[i].left_fork = philos[i - 1].right_fork;
			printf("philo %d is eating\n", i + 1);
		}
		i++;
	}
	// change_odd_forks(philos, "unlock");
	i = 1;
	while (i <= philos->nb)
	{
		if(i % 2 == 0){
			philos[i].forks_count += 1;
			printf("philo %d has %d fork\n", i, philos[i].forks_count);
			philos[i - 1].forks_count -= 1;
		}
		i++;
	}
	return (NULL);
	//pair -> odd(w/out last) -> last
}

void	create(int nb_thread, t_philo *philos, pthread_t *threads)
{
	philos->index = 0;
	// if (nb_thread % 2 == 0)
	// {
		while(philos->index <= nb_thread)
		{
			pthread_create(&threads[philos->index], NULL, pair_routine,(void *)philos);
			philos->index++;
		}
	// }
}

int	create_threads(int nb_thread, t_philo *philos)
{
	pthread_t	threads[nb_thread];

	philos->index = 0;
	create(nb_thread, philos, threads);
	join_threads(nb_thread, philos, threads);
	return (0);
}

int	parsing_infos(t_philo *philos, char **av)
{
	int	i;

	i = 0;
	while(av[i])
	{
		if(overflow(av[i]))
		{
			write(2, "Error\n", 6);
			return (1);
		}
		i++;
	}
	philos->nb = ft_atoi(av[1]);
	philos->die_time = ft_atoi(av[2]);
	philos->eat_time = ft_atoi(av[3]);
	philos->sleep_time = ft_atoi(av[4]);
	return (0);
}


int	main(int ac, char **av)
{
	t_philo philos;
	if(!(ac == 5 || ac == 6))
		return (0);
	if(parsing_infos(&philos, av))
		return (1);
	// pthread_mutex_init(&philos.mutex, NULL);
	// pthread_t threads[philos.philo];
	
	printf("Number of philosophers = %d\n", philos.nb);
	printf("time to die = %ldms\n", philos.die_time);
	printf("time to eat = %ldms\n", philos.eat_time);
	printf("time to sleep = %ldms\n", philos.sleep_time);
	if(ac == 5)
	{
		while(1){
			printf("Routine:\n");
			gettimeofday(&philos.start, NULL);
			create_threads(philos.nb , &philos);
			if(get_end(&philos) > philos.die_time)
			{
				printf("%ldms : philo %d died\n",get_end(&philos), philos.index);
				exit(0);
			}
		}
	}
	if(ac == 6)
	{
		int loop = atoi(av[5]);
		while(loop > 0)
		{
			printf("Before thread\n");
			gettimeofday(&philos.start, NULL);
			create_threads(philos.nb, &philos);
			if(get_end(&philos) > philos.die_time)
			{
				printf("%ldms : philo %d died\n",get_end(&philos), philos.index);
				exit(0);
			}
			loop--;
		}
	}
	// impaire philos time to die = time to eat *2 + time to sleep + 10ms pour ne pas mourir
	// paires philos time to die = time to eat + time to sleep + 10ms pour ne pas mourir
	// Exemples 5 ttd = 610ms tte = 200ms tts = 200ms 
	printf("threads are supposed to have spoken\n");
	return (0);
}
