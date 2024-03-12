/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:18:58 by wnocchi           #+#    #+#             */
/*   Updated: 2024/03/12 15:26:46 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
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
	long time = (philos->end.tv_sec - philos->start.tv_sec);
	long micros = ((time * 1000000) + philos->end.tv_usec) - (philos->start.tv_usec);
	return (micros / 1000);
}

// long	get_start(t_philo *philos)
// {
// 	long	start;
// 	gettimeofday(&philos->start, NULL);
// 	return (start);
// }

int		eat_routine(t_philo *philos)
{
	long	end;
	// long	start;

	// start = get_start(philos);
	// gettimeofday(&philos->start, NULL);
	// usleep(philos->eat_time * 1000);
	end = get_end(philos);
	if (end > philos->die_time)
	{
		printf("%ld ms: thread %d died :(\n", end, philos->philo);
		return (1);
	}
	// printf("%ldms philo %d eating\n", end, philos->philo);
	return (0);
}

int sleeping_routine(t_philo *philos)
{
	long end;
	// long	start;

	// start = get_start(philos);
	// gettimeofday(&philos->start, NULL);
	// usleep(philos->sleep_time * 1000);
	end = get_end(philos);
	if (end > philos->die_time)
	{
		printf("%ld ms: thread %d died :(\n", end, philos->philo);
		return (1);
	}
	// printf("%ldms philo %d sleeping\n", end, philos->philo);
	return (0);
}

int	thinking_routine(t_philo *philos)
{
	long end;
	// long	start;

	// start = get_start(philos);
	// gettimeofday(&philos->start, NULL);
	// usleep(philos->sleep_time * 1000);
	end = get_end(philos);
	if (end > philos->die_time)
	{
		printf("%ld ms: thread %d died :(\n", end, philos->philo);
		philos->philo--;
		return (1);
	}
	// printf("%ldms philo %d sleeping\n", end, philos->philo);
	return (0);
}

void	*thread_routine(void *arg)
{
	long end;
	t_philo *philos = (t_philo *)arg;
	
	// pthread_mutex_lock(&philos->mutex);
	gettimeofday(&philos->start, NULL);
	end = get_end(philos);
	printf("%ldms philo %d take a fork\n", end, philos->philo);
	// pthread_mutex_unlock(&philos->mutex);
	if(eat_routine(philos))
		return (NULL);
	end = get_end(philos);
	printf("%ldms philo %d eating\n", end, philos->philo);
	if(sleeping_routine(philos))
		return (NULL);
	end = get_end(philos);
	printf("%ldms philo %d sleeping\n", end, philos->philo);
	return (NULL);
}

void *print_thread(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	// usleep(200000);
	printf("this is philo %d\n", philos->philo);
	return (NULL);
}

int	create_threads(int nb_thread, t_philo *philos)
{
	pthread_t	threads[nb_thread];
	long		end;
	
	// threads = malloc(sizeof(pthread_t) * nb_thread);
	int i = 0;
	philos->philo = 0;
	while(i < nb_thread)
	{
		philos->philo++;
		pthread_create(&threads[i], NULL, thread_routine,(void *)philos);
		i++;
	}
	i = 0;
	philos->philo = 0;
	while(i < nb_thread){
		gettimeofday(&philos->start, NULL);
		philos->philo++;
		pthread_join(threads[i++], NULL);
		end = get_end(philos);
	}
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
	// philos->philo = ft_atoi(av[1]);
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
	
	printf("Number of philosophers = %d\n", philos.philo);
	printf("time to die = %ldms\n", philos.die_time);
	printf("time to eat = %ldms\n", philos.eat_time);
	printf("time to sleep = %ldms\n", philos.sleep_time);
	if(ac == 5)
	{
		while(1){
			printf("Routine:\n");
			create_threads(ft_atoi(av[1]), &philos);
			// usleep(300000);
				// pthread_join(threads, NULL);
			// exit(0);
		}
		// pthread_mutex_destroy(&philos.mutex);
	}
	if(ac == 6)
	{
		int loop = atoi(av[5]);
		while(loop > 0)
		{
			philos.philo = atoi(av[1]);
			printf("Before thread\n");
			if(create_threads(philos.philo, &philos))
				exit(0);	
			loop--;
		}
		pthread_mutex_destroy(&philos.mutex);
	}
	// impaire philos time to die = time to eat *2 + time to sleep + 10ms pour ne pas mourir
	// paires philos time to die = time to eat + time to sleep + 10ms pour ne pas mourir
	// Exemples 5 ttd = 610ms tte = 200ms tts = 200ms 
	printf("threads are supposed to have spoken\n");
	return (0);
}
