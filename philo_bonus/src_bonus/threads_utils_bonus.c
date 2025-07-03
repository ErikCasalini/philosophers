/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:58:47 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/03 10:16:19 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "philosophers_bonus.h"

static long long	hunger_counter(t_philo *philo, t_sem *semaphores)
{
	struct timeval	current_time;
	long long		ms_diff;

	semlock_gettimeofday(&current_time, semaphores->time);
	sem_wait(semaphores->time);
	ms_diff = get_ms_diff(philo->started_eat, current_time);
	sem_post(semaphores->time);
	return (ms_diff);
}

static void	dynamic_usleep(int total_philo)
{
	if (total_philo >= 20)
		usleep(80 * total_philo);
	else
		usleep(400);
}

static void	*d_tracker_routine(void *arg_lst)
{
	t_threads_args	*args;

	args = (t_threads_args *)arg_lst;
	while (1)
	{
		if (is_death_flag(args->philo, args->semaphores))
			return (NULL);
		if (hunger_counter(args->philo,
				args->semaphores) >= args->philo->tt_die)
		{
			sem_wait(args->semaphores->print);
			if (!is_death_flag(args->philo, args->semaphores)
				&& (args->philo->eat_max == -1
					|| args->philo->meals_eaten < args->philo->eat_max))
				printf("%lld %d died\n", curr_timestamp(args->philo->start_time,
						args->semaphores->time), args->philo->philo_num);
			sem_post(args->semaphores->death_occurred);
			dynamic_usleep(args->philo->total_philo);
			sem_post(args->semaphores->print);
			return (NULL);
		}
		usleep(250);
	}
	return (NULL);
}

static void	*d_setter_routine(void *arg_lst)
{
	t_threads_args	*args;

	args = (t_threads_args *)arg_lst;
	sem_wait(args->semaphores->death_occurred);
	sem_post(args->semaphores->death_occurred);
	sem_wait(args->semaphores->meals_eaten);
	if (args->philo->eat_max == -1
		|| args->philo->meals_eaten < args->philo->eat_max)
	{
		sem_post(args->semaphores->meals_eaten);
		sem_wait(args->semaphores->death_flag);
		args->philo->death_flag = 1;
		sem_post(args->semaphores->death_flag);
	}
	else
		sem_post(args->semaphores->meals_eaten);
	return (NULL);
}

void	create_threads(t_threads_args *args,
		pthread_t *d_tracker, pthread_t *d_setter)
{
	if (pthread_create(d_tracker, NULL, d_tracker_routine, args) == ERROR)
	{
		sem_post(args->semaphores->death_occurred);
		close_semaphores(args->semaphores);
		ft_putstr_fd("Thread creation error:"
			" simulation will be interrupted\n", 2);
		exit(ERROR);
	}
	if (pthread_create(d_setter, NULL, d_setter_routine, args) == ERROR)
	{
		sem_post(args->semaphores->death_occurred);
		sem_wait(args->semaphores->death_flag);
		args->philo->death_flag = 1;
		sem_post(args->semaphores->death_flag);
		pthread_join(*d_tracker, NULL);
		close_semaphores(args->semaphores);
		ft_putstr_fd("Thread creation error:"
			" simulation will be interrupted\n", 2);
		exit(ERROR);
	}
}
