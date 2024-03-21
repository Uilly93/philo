/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:18:58 by wnocchi           #+#    #+#             */
/*   Updated: 2024/03/21 16:32:36 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdbool.h>
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
	gettimeofday(&philos->infos->end, NULL);
	// long time = (philos->end.tv_sec - philos->start.tv_sec);
	// long micros = ((time * 1000000) + philos->end.tv_usec) - (philos->start.tv_usec);
	// return (micros / 1000);
	long start = ((philos->infos->start.tv_sec * 1000000)
	+ philos->infos->start.tv_usec) / 1000;
	long end = ((philos->infos->end.tv_sec * 1000000)
	+ philos->infos->end.tv_usec) / 1000;
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
		if(philos[philos->index].eaten == false)
		{
			usleep(philos->infos->eat_time * 1000);
			printf("%ld ms: philo %d eating\n", get_end(philos), philos->index + 1);
			philos[philos->index].eaten = true;
		}
		philos->index++;
		return (0);
}

int sleeping_routine(t_philo *philos)
{
	if(philos[philos->index].slept == false)
	{
		usleep(philos->infos->sleep_time * 1000);
		printf("%ld ms: philo %d sleeping\n", get_end(philos), philos->index + 1);
		philos[philos->index].slept = true;
	}
	philos->index++;
	return (0);
}

int	thinking_routine(t_philo *philos)
{
	printf("philo %d thinking\n", philos->index);
	// printf("%ldms philo %d sleeping\n", end, philos->philo);
	return (0);
}

void	thread_routine(t_philo *philos)
{
	if(eat_routine(philos))
		return ;
	if(sleeping_routine(philos))
		return ;
	return ;
}

void *print_thread(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	// usleep(200000);
	printf("this is philo %d\n", philos->index);
	return (NULL);
}

void	join_threads(int nb_thread, pthread_t *threads)
{
	int		i;
	
	i = 0;
	// philos->index = 0;
	while(i < nb_thread)
		pthread_join(threads[i++], NULL);
}

void	free_mutex(t_philo *philos)
{
	int i;

	i = 0;
	while(i < philos->infos->nb)
	{
		if(philos[i].right_fork)
			free(philos[i].right_fork);
		i++;
	}
	return ;
}

int	init_mutex(t_philo *philos)
{
	int i;

	i = 0;
	while(i < philos->infos->nb)
	{
		philos[i].right_fork = malloc(sizeof(pthread_mutex_t));
		if(!philos[i].right_fork)
		{
			free_mutex(philos);
			return (1);
		}
		i++;
	}
	i = 0;
	while(i < philos->infos->nb)
	{
		pthread_mutex_init(philos[i].right_fork, NULL);
		i++;
	}
	return (0);
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
		while (i < philos->infos->nb)
		{
			if(i % 2 != 0)
				pthread_mutex_lock(philos[i].right_fork);
			i++;
		}
	}
	else if (ft_strcmp(s, "unlock") == 0)
	{
		while (i < philos->infos->nb)
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
		while (i < philos->infos->nb)
		{
			if(i % 2 == 0)
				pthread_mutex_lock(philos[i].right_fork);
			i++;
		}
	}
	else if (ft_strcmp(s, "unlock") == 0)
	{
		while (i < philos->infos->nb)
		{
			if(i % 2 == 0)
				pthread_mutex_unlock(philos[i].right_fork);
			i++;
		}
	}
}

// bool	check_forks(t_philo *philos)
// {
// 	int i;

// 	i = 0;
// 	while(i < philos->nb)
// 	{
// 		if(philos[i].forks_count == 2)
// 		{
// 			printf("philos %d is eating", i + 1);
// 			philos[i].eaten = true;
// 		}
// 	}
// }

void	*pair_routine(void *args)
{
	t_philo *philos;
	int	i;
	
	philos = (t_philo *)args;
	i = 0;
	
	philos->forks_count = 1;
	// philos->index = 0;
	philos->eaten = false;
	philos->slept = false;
	if(init_mutex(philos))
		return (NULL);
	while (i < philos->infos->nb)
	{
		if(i % 2 == 0)
		{
			if(i == 0)
				philos[0].left_fork = philos[philos->infos->nb].right_fork;
			philos[i].left_fork = philos[i - 1].right_fork;
			// printf("%ld ms: ", get_end(philos));
			printf("%ld ms: philo %d take a fork\n", get_end(philos), i + 1);
			i++;
		}
		else
			i++;
	}
	change_odd_forks(philos, "lock");
	thread_routine(philos);
	change_odd_forks(philos, "unlock");
	return (NULL);
}

void	create(int nb_thread, t_philo *philos, pthread_t *threads)
{
	int i = 0;
	philos->index = 0;
	// if (nb_thread % 2 == 0)
	// {
		while(i < nb_thread)
		{
			pthread_create(&threads[i], NULL, print_thread,(void *)&philos[i]);
			// philo
			// usleep(1);
			philos[i].index++;
			i++;
		}
	// }
}

int	create_threads(int nb_thread, t_philo *philos)
{
	pthread_t	threads[nb_thread];
	
	create(nb_thread, philos, threads);
	join_threads(nb_thread, threads);
	return (0);
}

int	parsing_infos(t_philo *philos, t_simu *infos, char **av)
{
	int	i;
	(void)philos;

	i = 0;
	// philos = malloc(sizeof(t_philo *)* ft_atoi(av[1]));
	// philos->infos = malloc(sizeof(t_simu));
	// printf("%d\n", ft_atoi(av[5]));
	while(av[i])
	{
		if(overflow(av[i]))
		{
			write(2, "Error\n", 6);
			// free(philos);
			return (1);
		}
		i++;
	}
	infos->nb = ft_atoi(av[1]);
	if(i == 6)	
		infos->loop = ft_atoi(av[5]);
	else
		infos->loop = 0;
			// if(infos->loop < 1)
			// 	return (1);
	i = 0;
	while (i < infos->nb)
	{
		philos[i].index = i;
		i++;
	}
	infos->die_time = ft_atoi(av[2]);
	infos->eat_time = ft_atoi(av[3]);
	infos->sleep_time = ft_atoi(av[4]);
	return (0);
}

// void	loop_thread(t_philo philos, int nb)
// {
// 	while(nb < 0)
// 		create_threads(int nb_thread, t_philo *philos)
// }


int	main(int ac, char **av)
{
	t_philo philos;
	t_simu	infos;
	// pthread_t	threads[ft_atoi(av[1])];
	// philos = NULL;
	// t_philo *philos = malloc(sizeof(t_philo *)* ft_atoi(av[1]));
	if(!(ac == 5 || ac == 6))
		return (0);
	if(parsing_infos(&philos, &infos, av))
		return (1);
	printf("Number of philosophers = %d\n", infos.nb);
	printf("time to die = %ldms\n", infos.die_time);
	printf("time to eat = %ldms\n", infos.eat_time);
	printf("time to sleep = %ldms\n", infos.sleep_time);
	if(ac == 5)
	{
		gettimeofday(&infos.start, NULL);
		create_threads(infos.nb, &philos);
		while(1){
			// printf("Routine:\n");
			// if(get_end(&philos) > philos.die_time)
			// {
			// 	printf("%ldms : philo %d died\n",get_end(&philos), philos.index);
			// 	exit(0);
			// }
		}
	}
	if(ac == 6)
	{
		printf("Number of loop = %d\n", infos.loop);
		// int loop = atoi(av[5]);
		gettimeofday(&infos.start, NULL);
		create_threads(infos.nb, &philos);
		while(infos.loop > 0)
		{
			// join_threads(infos.nb, threads);
			// printf("Before thread\n");
			// if(get_end(&philos) > philos.die_time)
			// {
			// 	printf("%ldms : philo %d died\n",get_end(&philos), philos.index);
			// 	exit(0);
			// }
			infos.loop--;
		}
	}
	// impaire philos time to die = time to eat *2 + time to sleep + 10ms pour ne pas mourir
	// paires philos time to die = time to eat + time to sleep + 10ms pour ne pas mourir
	// Exemples 5 ttd = 610ms tte = 200ms tts = 200ms 
	printf("threads are supposed to have spoken\n");
	return (0);
}
