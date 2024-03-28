/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:18:58 by wnocchi           #+#    #+#             */
/*   Updated: 2024/03/28 16:49:14 by wnocchi          ###   ########.fr       */
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

long	get_end(t_philo *philo)
{
	gettimeofday(&philo->infos->end, NULL);
	long start = ((philo->infos->start.tv_sec * 1000000)
	+ philo->infos->start.tv_usec) / 1000;
	long end = ((philo->infos->end.tv_sec * 1000000)
	+ philo->infos->end.tv_usec) / 1000;
	return (end - start);
}

int		eat_routine(t_philo *philo)
{
		if(philo[philo->index].eaten == false)
		{
			usleep(philo->infos->eat_time * 1000);
			printf("%ld ms: philo %d eating\n", get_end(philo), philo->index + 1);
			philo[philo->index].eaten = true;
		}
		philo->index++;
		return (0);
}

int sleeping_routine(t_philo *philo)
{
	if(philo[philo->index].slept == false)
	{
		usleep(philo->infos->sleep_time * 1000);
		printf("%ld ms: philo %d sleeping\n", get_end(philo), philo->index + 1);
		philo[philo->index].slept = true;
	}
	philo->index++;
	return (0);
}

int	thinking_routine(t_philo *philo)
{
	printf("philo %d thinking\n", philo->index);
	// printf("%ldms philo %d sleeping\n", end, philo->philo);
	return (0);
}

void	thread_routine(t_philo *philo)
{
	if(eat_routine(philo))
		return ;
	if(sleeping_routine(philo))
		return ;
	return ;
}

void	join_threads(int nb_thread, t_philo *philo)
{
	int		i;
	
	i = 0;
	// philo->index = 0;
	while(i < nb_thread)
		pthread_join(philo[i++].threads, NULL);
}

void	give_forks(t_philo *philo)
{
	int i;

	i = 0;
	while(i < philo->infos->nb)
	{
		if(i == 0)
			philo[i].left_fork = philo[philo->infos->nb - 1].right_fork;
		else
			philo[i].left_fork = philo[i - 1].right_fork;
		i++;
	}
}

void	free_mutexs(t_philo *philo);

int	init_mutex(t_philo *philo)
{
	int i;

	i = 0;
	while(i < philo->infos->nb)
	{
		philo[i].right_fork = malloc(sizeof(pthread_mutex_t));
		if(!philo[i].right_fork)
		{
			free_mutexs(philo);
			return (1);
		}
		pthread_mutex_init(philo[i].right_fork, NULL);
		i++;
	}
	give_forks(philo);
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

void	change_odd_forks(t_philo *philo, char *s)
{
	int i;

	i = 0;
	if(ft_strcmp(s, "lock") == 0)
	{
		while (i < philo->infos->nb)
		{
			if(i % 2 != 0)
				pthread_mutex_lock(philo[i].right_fork);
			i++;
		}
	}
	else if (ft_strcmp(s, "unlock") == 0)
	{
		while (i < philo->infos->nb)
		{
			if(i % 2 != 0)
				pthread_mutex_unlock(philo[i].right_fork);
			i++;
		}
	}
}

void	change_pair_forks(t_philo *philo, char *s)
{
	int i;

	i = 0;
	if(ft_strcmp(s, "lock") == 0)
	{
		while (i < philo->infos->nb)
		{
			if(i % 2 == 0)
				pthread_mutex_lock(philo[i].right_fork);
			i++;
		}
	}
	else if (ft_strcmp(s, "unlock") == 0)
	{
		while (i < philo->infos->nb)
		{
			if(i % 2 == 0)
				pthread_mutex_unlock(philo[i].right_fork);
			i++;
		}
	}
}

void	take_fork(t_philo *philo, char *state)
{
	if(ft_strcmp(state, "lock") == 0)
	{
		if(philo->index == 0)
		{
			// printf("philo %d takes philo's %d fork at %p\n", philo->index, philo->infos->nb, philo[philo->infos->nb - 1].right_fork); // test
			pthread_mutex_lock(philo[philo->infos->nb - 1].right_fork); // need test
		}
		else
		{
			// printf("philo %d takes philo's %d fork\n", philo->index, philo->index - 1); // test
			pthread_mutex_lock(philo[philo->index - 1].right_fork);
		}
	}
	else if(ft_strcmp(state, "unlock"))
		{
		if(philo->index == 0)
		{
			// printf("philo %d takes philo's %d fork\n", philo->index, philo->infos->nb - 1); // test
			pthread_mutex_unlock(philo[philo->infos->nb].right_fork);
		}
		else
		{
			// printf("philo %d takes philo's %d fork\n", philo->index, philo->index - 1); // test
			pthread_mutex_unlock(philo[philo->index - 1].right_fork);
		}
	}
}

void	*pair_routine(void *args)
{
	t_philo *philo;
	
	philo = (t_philo *)args;
	// if(philo->forks_count == 2)
	// if(philo->index % 2 != 0)
	// {
		// take_fork(philo, "lock");
		printf("philo %d:\n", philo->index + 1);
		// take_fork(philo, "unlock");
	// }
	return (NULL);
}

void	create(int nb_thread, t_philo *philo)
{
	int i = 0;
	// if (nb_thread % 2 == 0)
	// {
		while(i < nb_thread)
		{
			pthread_create(&philo[i].threads, NULL, pair_routine,(void *)&philo[i]);
			i++;
		}
		return ;
	// }
}

int	create_threads(int nb_thread, t_philo *philo)
{
	create(nb_thread, philo);
	join_threads(nb_thread, philo);
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
			// free(philo);
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

int	parsing_infos(t_philo **philo, t_infos *infos, char **av)
{
	int	i;
	pthread_mutex_t right_fork;

	i = 0;
	if (check_overflow(infos, av))
		return(1);
	infos->nb = ft_atoi(av[1]);
	*philo = malloc(sizeof(t_philo)* infos->nb);
	// printf("%d philo = %p\n", __LINE__, *philo);
	infos->die_time = ft_atoi(av[2]);
	infos->eat_time = ft_atoi(av[3]);
	infos->sleep_time = ft_atoi(av[4]);
	// printf("%d philo = %p\n", __LINE__, *philo);
	while (i < infos->nb)
	{
		(*philo)[i].forks_count = 1;
		(*philo)[i].infos = infos;
		(*philo)[i].index = i;
		(*philo)[i].right_fork = &right_fork;
		i++;
	}
	if(init_mutex(*philo))
		return (1);
	// printf("%d fork = %p\n", __LINE__, (*philo)->right_fork);
	// printf("%d philo = %p\n", __LINE__, *philo);
	return (0);
}

void	free_mutexs(t_philo *philo)
{
	int i;

	i = 0;
	while(i < philo->infos->nb)
	{
		if(philo[i].right_fork)
			free(philo[i].right_fork);
		i++;
	}
	free(philo);
	return ;
}

int	main(int ac, char **av)
{
	t_philo *philo;
	t_infos	infos;
	
	if(!(ac == 5 || ac == 6))
		return (0);
	if(parsing_infos(&philo, &infos, av))
		return (1);
	// printf("%d philo = %p\n", __LINE__, philo);
	// printf("%d fork = %p\n", __LINE__, philo->right_fork);
	// printf("Number of philoophers = %d\n", infos.nb);
	// printf("time to die %s  = ⏱️  %ldms\n", "\U0001FAE5", infos.die_time);
	// printf("time to eat %s = ⏱️  %ldms\n", "\U0001F60B", infos.eat_time);
	// printf("time to sleep %s = ⏱️  %ldms\n","\U0001F62A", infos.sleep_time);
	if(ac == 5)
	{
		gettimeofday(&infos.start, NULL);
		create_threads(infos.nb, philo);
		while(1)
		{
		}
	}
	if(ac == 6)
	{
		gettimeofday(&infos.start, NULL);
		create_threads(infos.nb, philo);
		for(int i = 10; i > 0; i--)
		{}
	}
	free_mutexs(philo);
	return (0);
	// impaire philo time to die = time to eat *2 + time to sleep + 10ms pour ne pas mourir
	// paires philo time to die = time to eat + time to sleep + 10ms pour ne pas mourir
	// Exemples 5 ttd = 610ms tte = 200ms tts = 200ms 
	// printf("threads are supposed to have spoken\n");
}
