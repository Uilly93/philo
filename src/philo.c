/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:18:58 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/11 19:16:42 by wnocchi          ###   ########.fr       */
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
	long	start;
	long	end;
	
	gettimeofday(&philo->infos->end, NULL);
	start = ((philo->infos->start.tv_sec * 1000000)
	+ philo->infos->start.tv_usec) / 1000;
	end = ((philo->infos->end.tv_sec * 1000000)
	+ philo->infos->end.tv_usec) / 1000;
	return (end - start);
}

void	join_threads(int nb_thread, t_philo *philo)
{
	int		i;
	
		i = 0;
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
			philo[i].l_fork = philo[philo->infos->nb - 1].r_fork;
		else
			philo[i].l_fork = philo[i - 1].r_fork;
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
		philo[i].r_fork = malloc(sizeof(pthread_mutex_t));
		if(!philo[i].r_fork)
		{
			free_mutexs(philo);
			return (1);
		}
		pthread_mutex_init(philo[i].r_fork, NULL);
		i++;
	}
	give_forks(philo);
	return (0);
}

void	lock_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	pthread_mutex_lock(philo->l_fork);
	return ;
}

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	return ;
}


void	eat_think_sleep(t_philo *philo)
{
	if(philo->infos->dead)
		return ;
	lock_forks(philo);
	pthread_mutex_lock(&philo->infos->print);
	printf("%ld %d is eating\n",get_end(philo), philo->index +1);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->infos->print);
	usleep(philo->infos->eat_time * 1000);
	philo->last_meal = get_end(philo);
	unlock_forks(philo);
	pthread_mutex_lock(&philo->infos->print);
	printf("%ld %d is sleeping\n",get_end(philo), philo->index +1);
	pthread_mutex_unlock(&philo->infos->print);
	usleep(philo->infos->sleep_time * 1000);
	pthread_mutex_lock(&philo->infos->print);
	printf("%ld %d is thinking\n",get_end(philo), philo->index +1);
	pthread_mutex_unlock(&philo->infos->print);
}

// void	think_sleep(t_philo *philo)
// {
// }

void	*routine(void *args)
{
	t_philo *philo;
	
	philo = (t_philo *)args;
	if (philo->index % 2 != 0)
		usleep(philo->infos->eat_time * 1000);
	if (philo->infos->nb % 2 != 0 && philo->index == philo->infos->nb - 1)
		usleep((philo->infos->sleep_time + philo->infos->eat_time) * 1000);
	if(philo->infos->loop)
	{
		while (philo->eat_count < philo->infos->nb_loop)
		{
			if(philo->infos->dead)
				return (NULL);
			eat_think_sleep(philo);
		}
		return (NULL);	
	}
	while (1)
	{
		if(philo->infos->dead)
			return (NULL);
		eat_think_sleep(philo);
	}
	return (NULL);
}


int	check_death(t_philo *philo) // fix
{
	long	eat_limit;
	int		i;

	while (1)
	{
		i = 0;
		while(i < philo->infos->nb)
		{
			eat_limit = get_end(philo) - philo[i].last_meal;
			// printf("%ld\n",eat_limit); // FIX CALCULATION
			if(eat_limit > philo->infos->die_time)
			{
				philo->infos->dead = true;
				pthread_mutex_lock(&philo->infos->print);
				printf("%ld philo %d died\n", get_end(philo), i + 1);
				pthread_mutex_unlock(&philo->infos->print);
				return (1);
			}
			i++;
		}
		usleep(1000);
	}
	return (0);
}

void	create(int nb_thread, t_philo *philo)
{
	int i = 0;
		while(i < nb_thread)
		{
			pthread_create(&philo[i].threads, NULL, routine,(void *)&philo[i]);
			i++;
		}
		return ;
}
int	check_error(t_infos *infos)
{
	if(infos->die_time < 0)
		return (1);
	if(infos->eat_time < 0)
		return (1);
	if(infos->sleep_time < 0)
		return (1);
	if(infos->nb < 0)
		return (1);
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
			write(2, "Error\nArguments overflow\n", 25);
			return (1);
		}
		i++;
	}
	if (i == 6)
	{
		infos->loop = true;
		infos->nb_loop = ft_atoi(av[5]);
	}
	infos->finished = false;
	infos->nb = ft_atoi(av[1]);
	infos->die_time = ft_atoi(av[2]);
	infos->eat_time = ft_atoi(av[3]);
	infos->sleep_time = ft_atoi(av[4]);
	if (check_error(infos) || (infos->loop == true && infos->nb_loop <= 0))
		return(write(2, "Error\nInvalid arguments\n", 24),1);
	return (0);
}


int	parsing_infos(t_philo **philo, t_infos *infos, char **av)
{
	int	i;
	
	i = 0;
	infos->loop = false;
	infos->dead = false;
	if (check_overflow(infos, av))
		return(1);
	*philo = malloc(sizeof(t_philo)* infos->nb);
	if(!*philo)
		return (1);
	pthread_mutex_init(&infos->print, NULL);
	while (i < infos->nb)
	{
		(*philo)[i].infos = infos;
		(*philo)[i].index = i;
		(*philo)[i].last_meal = 0;
		(*philo)[i].eat_count = 0;
		i++;
	}
	if(init_mutex(*philo))
		return (1);
	return (0);
}

void	free_mutexs(t_philo *philo)
{
	int i;

	i = 0;
	while(i < philo->infos->nb)
	{
		if(philo[i].r_fork)
			free(philo[i].r_fork);
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
	// printf("loop : %d", infos.loop);
	gettimeofday(&infos.start, NULL);
	create(infos.nb, philo);
	if(check_death(philo) || infos.finished)
	{
		join_threads(infos.nb, philo);
		free_mutexs(philo);
		return (1);
	}
	return (0);
}
	// impaire philo time to die = time to eat *2 + time to sleep + 10ms pour ne pas mourir
	// paires philo time to die = time to eat + time to sleep + 10ms pour ne pas mourir
	// Exemples 5 ttd = 610ms tte = 200ms tts = 200ms 
	// printf("threads are supposed to have spoken\n");
	// printf("%d philo = %p\n", __LINE__, philo);
	// printf("%d fork = %p\n", __LINE__, philo->r_fork);
	// printf("Number of philoophers = %d\n", infos.nb);
	// printf("time to die %s  = ⏱️  %ldms\n", "\U0001FAE5", infos.die_time);
	// printf("time to eat %s = ⏱️  %ldms\n", "\U0001F60B", infos.eat_time);
	// printf("time to sleep %s = ⏱️  %ldms\n","\U0001F62A", infos.sleep_time);
