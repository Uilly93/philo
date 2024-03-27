/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:18:58 by wnocchi           #+#    #+#             */
/*   Updated: 2024/03/27 14:11:58 by wnocchi          ###   ########.fr       */
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
	long start = ((philos->infos->start.tv_sec * 1000000)
	+ philos->infos->start.tv_usec) / 1000;
	long end = ((philos->infos->end.tv_sec * 1000000)
	+ philos->infos->end.tv_usec) / 1000;
	return (end - start);
}

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
	(void) philos;
	// t_infos *infos;
	// infos = philos->infos;
	// philos->infos = malloc(sizeof(t_infos *));
    // 🍽️
	// infos = malloc(sizeof(t_infos));
	// usleep(200000);
	printf("⏱️  %ldms : this is philo %d\n", get_end(philos), philos->index + 1);
	// printf("nb philos is %d\n", philos->infos->nb);
	return (NULL);
}

void	join_threads(int nb_thread, t_philo *philos)
{
	int		i;
	
	i = 0;
	// philos->index = 0;
	while(i < nb_thread)
		pthread_join(philos[i++].threads, NULL);
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

void	take_fork(t_philo *philo, int num, char *state)
{
	t_infos	*infos;
	
	if(ft_strcmp(state, "lock") == 0)
	{
		if(num == 0)
		{
			philo[0].left_fork = philo[infos->nb].right_fork;
			pthread_mutex_lock(philo[infos->nb].right_fork); // need test 
		}	
		philo[num].left_fork = philo[num - 1].right_fork;
		pthread_mutex_lock(philo[num - 1].right_fork);
	}
	if(ft_strcmp(state, "unlock"))
		{
		if(num == 0)
			pthread_mutex_unlock(philo[infos->nb].right_fork); // need test 
		pthread_mutex_unlock(philo[num - 1].right_fork);
	}
}

void	*pair_routine(void *args)
{
	t_philo *philos;
	// t_infos *infos = NULL;
	int	i;
	
	philos = (t_philo *)args;
	i = 0;
	
	// philos->forks_count = 1;
	// philos->index = 0;
	// philos->eaten = false;
	// philos->slept = false;
	if(init_mutex(philos))
		return (NULL);
	while(1)
	{
		take_fork(philo[i], i, char *state)
		change_odd_forks(philos, "lock");
	}
	return (NULL);
}

void	create(int nb_thread, t_philo *philos)
{
	int i = 0;
	// if (nb_thread % 2 == 0)
	// {
		while(i < nb_thread)
		{
			pthread_create(&philos[i].threads, NULL, pair_routine,(void *)&philos[i]);
			i++;
		}
		return ;
	// }
}

int	create_threads(int nb_thread, t_philo *philos)
{
	// pthread_t	threads[nb_thread];
	
	create(nb_thread, philos);
	join_threads(nb_thread, philos);
	return (0);
}

int check_overflow(t_infos *infos, char **av)
{
	int i;
	
	i = 0;
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
	if(i == 6)
		infos->loop = ft_atoi(av[5]);
	else
		infos->loop = 0;
	return (0);
}

int	parsing_infos(t_philo **philos, t_infos *infos, char **av)
{
	int	i;
	
	i = 0;
	if (check_overflow(infos, av))
		return(1);
	infos->nb = ft_atoi(av[1]);
	*philos = malloc(sizeof(t_philo)* infos->nb);
	printf("%d philos = %p\n", __LINE__, *philos);
	infos->die_time = ft_atoi(av[2]);
	infos->eat_time = ft_atoi(av[3]);
	infos->sleep_time = ft_atoi(av[4]);
	printf("%d philos = %p\n", __LINE__, *philos);
	while (i < infos->nb)
	{
		// philos[i] = malloc(sizeof(t_philo));
	// printf("%d philos = %p\n", __LINE__, philos);
		(*philos)[i].forks_count = 1;
		(*philos)[i].infos = infos;
		(*philos)[i].index = i;
		// philos[i] = philos;
		i++;
	}
	printf("%d philos = %p\n", __LINE__, *philos);
	return (0);
}


int	main(int ac, char **av)
{
	t_philo *philos = NULL;
	
	t_infos	infos;
	// pthread_t	threads[ft_atoi(av[1])];
	// philos = NULL;
	// philos = malloc(sizeof(t_philo *));
	if(!(ac == 5 || ac == 6))
		return (0);
	if(parsing_infos(&philos, &infos, av))
		return (1);
	printf("%d philos = %p\n", __LINE__, philos);
	// printf("Number of philosophers = %d\n", infos.nb);
	// printf("time to die %s  = ⏱️  %ldms\n", "\U0001FAE5", infos.die_time);
	// printf("time to eat %s = ⏱️  %ldms\n", "\U0001F60B", infos.eat_time);
	// printf("time to sleep %s = ⏱️  %ldms\n","\U0001F62A", infos.sleep_time);
	if(ac == 5)
	{
		gettimeofday(&infos.start, NULL);
		create_threads(infos.nb, philos);
		while(1)
		{
			// join_threads(infos.nb, philos);
			// printf("⏱️  %ldms\n", get_end(&philos));
		}
	}
	if(ac == 6)
	{
		// printf("Number of loop = %d\n", infos.loop);
		// int loop = atoi(av[5]);
		gettimeofday(&infos.start, NULL);
		create_threads(infos.nb, philos);
		for(int i = 10; i > 0; i--)
		{}
	}
	free(philos);
	return (0);
	// impaire philos time to die = time to eat *2 + time to sleep + 10ms pour ne pas mourir
	// paires philos time to die = time to eat + time to sleep + 10ms pour ne pas mourir
	// Exemples 5 ttd = 610ms tte = 200ms tts = 200ms 
	// printf("threads are supposed to have spoken\n");
}
