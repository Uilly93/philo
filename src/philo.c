/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:18:58 by wnocchi           #+#    #+#             */
/*   Updated: 2024/04/15 18:57:17 by wnocchi          ###   ########.fr       */
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

void	set_as_finished(t_philo *philo);

bool only_digit_string(char *s)
{
    int i;

    i = 0;
    while(s[i])
    {
        if(s[i] >= '0' && s[i] <= '9')
            i++;
        else
            return(true);
    }
    return(false);
}

bool overflow(char *s)
{
	int		i;
	int		sign;
	long	nbr;
	
	i = 0;
	sign = 1;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ' || s[i] == '+')
		i++;
	if (s[i] == '-')
	{
		sign *= -1;
		i++;
	}
	if (only_digit_string(s + i))
		return (true);
	while(s[i] >= '0' && s[i] <= '9')
	{
		nbr = (nbr * 10 + s[i++] - '0');
		if(nbr * sign > INT_MAX || nbr * sign < INT_MIN)
			return (true);
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
		nbr = (nbr * 10 + s[i] - '0');
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
		philo[i].acces = malloc(sizeof(pthread_mutex_t)); ///
		philo[i].r_fork = malloc(sizeof(pthread_mutex_t));
		if(!philo[i].r_fork || !philo[i].acces)
		{
			free_mutexs(philo);
			return (1);
		}
		pthread_mutex_init(philo[i].acces, NULL); ///
		pthread_mutex_init(philo[i].r_fork, NULL);
		i++;
	}
	give_forks(philo);
	return (0);
}

void	lock_forks(t_philo *philo)
{
	if(philo->r_fork && philo->r_fork)
	{
		if(philo->index % 2 == 0)
		{
			pthread_mutex_lock(philo->l_fork);
			pthread_mutex_lock(philo->r_fork);
		}
		else
		{
			pthread_mutex_lock(philo->r_fork);
			pthread_mutex_lock(philo->l_fork);
		}
	}
}

void	unlock_forks(t_philo *philo)
{
	if(philo->r_fork && philo->r_fork)
	{
		if(philo->index % 2 == 0)
		{
			pthread_mutex_unlock(philo->l_fork);
			pthread_mutex_unlock(philo->r_fork);
		}
		else
		{
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
		}
	}
}

int	stop_simulation(t_philo *philo)
{
	pthread_mutex_lock(&philo->infos->mutex);
	if(philo->eat_count == philo->infos->nb_loop ||
	philo->infos->dead || philo->infos->finished)
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->infos->mutex);
	return(0);
}

int	check_loop_eat(t_philo *philo)
{
	int i;
	int	count;

	count = 0;
	i = 0;
	while(i < philo->infos->nb) // creer un mutex pour chaque philo et le lock pour y  acceder
	{
		pthread_mutex_lock(&philo->infos->mutex);
		if(philo[i].eat_count == philo->infos->nb_loop)
			count++;
		pthread_mutex_unlock(&philo->infos->mutex);
		i++;
	}
	if (count == philo->infos->nb)
	{
		set_as_finished(philo);
		join_threads(philo->infos->nb, philo);
		pthread_mutex_lock(&philo->infos->mutex);
		printf("All philosophers ate %d times, end of the simulation\n",
		philo->infos->nb_loop);
		pthread_mutex_unlock(&philo->infos->mutex);
		return (1);
	}
	return (0);
}

int	check_dead(t_philo *philo)
{
	int i;

	i = 0;
	while(i < philo->infos->nb)
	{
		if(philo[i].infos->dead)
			return(1);
		i++;
	}
	return (0);
}

void	think_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->infos->mutex);
	printf("%ld %d is sleeping\n",get_end(philo), philo->index +1);
	pthread_mutex_unlock(&philo->infos->mutex);
	usleep(philo->infos->sleep_time * 1000);
	pthread_mutex_lock(&philo->infos->mutex);
	if(philo->infos->finished) // reduce
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		return ;
	}
	printf("%ld %d is thinking\n",get_end(philo), philo->index +1);
	pthread_mutex_unlock(&philo->infos->mutex);
}

void	eat_think_sleep(t_philo *philo)
{
	lock_forks(philo);
	pthread_mutex_lock(&philo->infos->mutex);
	if(philo->infos->finished) // reduce
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		unlock_forks(philo);
		return ;
	}
	printf("%ld %d is eating\n",get_end(philo), philo->index +1);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->infos->mutex);
	usleep(philo->infos->eat_time * 1000);
	pthread_mutex_lock(&philo->infos->mutex);
	philo->last_meal = get_end(philo);
	if(philo->infos->finished) // reduce
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		unlock_forks(philo);
		return ;
	}
	pthread_mutex_unlock(&philo->infos->mutex);
	unlock_forks(philo);
	think_sleep(philo);
}

void	set_as_finished(t_philo *philo)
{
	int i;
	
	i = 0;
	while (i < philo->infos->nb)
	{
		pthread_mutex_lock(&philo->infos->mutex);
		philo[i].infos->finished = true;
		pthread_mutex_unlock(&philo->infos->mutex);
		i++;
	}
}

int	who_died(t_philo *philo)
{
	int	i;
	
	i = 0;
	while(i < philo->infos->nb)
	{
		if(philo[i].infos->dead)
			return (i + 1);
	}
	return (0);
}

int	set_as_dead(t_philo *philo)
{
	int		i;
	long	eat_limit;

	i = 0;
	eat_limit = 0;
	while(i < philo->infos->nb)
	{
		pthread_mutex_lock(&philo->infos->mutex);
		eat_limit = get_end(philo) - philo[i].last_meal;
		pthread_mutex_unlock(&philo->infos->mutex);
		if(eat_limit > philo->infos->die_time || check_dead(philo))
		{
			set_as_finished(philo);
			pthread_mutex_lock(&philo->infos->mutex);
			philo[i].infos->dead = true;
			printf("%ld philo %d died\n", get_end(philo), who_died(philo));
			// who_died(philo);
			pthread_mutex_unlock(&philo->infos->mutex);
			join_threads(philo->infos->nb, philo);
			return (1);
		}
		i++;
	}
	return (0);
}

int	edge_case(t_philo *philo)
{
	pthread_mutex_lock(&philo->infos->mutex);
	// printf("nb philo = %d\n", philo->infos->nb);
	if(philo->infos->nb == 1)
	{
		pthread_mutex_unlock(&philo->infos->mutex);
		usleep(philo->infos->dead * 1000);
		return (1);
	}
	pthread_mutex_unlock(&philo->infos->mutex);
	if (philo->index % 2 != 0)
		usleep(philo->infos->eat_time * 1000);
	// pthread_mutex_lock(&philo->infos->mutex);
	if (philo->infos->nb % 2 != 0 && philo->index == philo->infos->nb - 1)
	// pthread_mutex_unlock(&philo->infos->mutex);
		usleep((philo->infos->sleep_time + philo->infos->eat_time) * 1000);
	// pthread_mutex_unlock(&philo->infos->mutex);
	return (0);
}

void	*routine(void *args)
{
	t_philo *philo;
	
	philo = (t_philo *)args;

	if(edge_case(philo))
		return(NULL);
	if(philo->infos->loop)
	{
		if(stop_simulation(philo))
			return (NULL);
		while (1)
		{
			pthread_mutex_lock(&philo->infos->mutex);
			if(philo->infos->finished)
			{
				pthread_mutex_unlock(&philo->infos->mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->infos->mutex);
			eat_think_sleep(philo);
		}
		return (NULL);	
	}
	while (1)
	{
		if(stop_simulation(philo))
			return (NULL);
		eat_think_sleep(philo);
	}
	return (NULL);
}


int	check_end(t_philo *philo)
{
	while (1)
	{
		if(check_loop_eat(philo))
			return(1);
		if(set_as_dead(philo))
			return (1);
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

	i = 1;
	while(av[i])
	{
		if(overflow(av[i]))
		{
			write(2, "Error\nInvalid arguments\n", 24);
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
	pthread_mutex_init(&infos->mutex, NULL);
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
		if(philo[i].acces)
			free(philo[i].acces);
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
	gettimeofday(&infos.start, NULL);
	create(infos.nb, philo);
	check_end(philo); // fix les erreurs de join 
	// join_threads(infos.nb, philo);
	free_mutexs(philo);
	return (0);
}
