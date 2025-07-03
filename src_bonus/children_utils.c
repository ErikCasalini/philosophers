/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:06:21 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/03 10:09:12 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "philosophers_bonus.h"

static int	start_thinking(t_philo *philo, t_sem *semaphores)
{
	usleep(250);
	if (is_death_flag(philo, semaphores))
		return (ERROR);
	semlock_printf("%lld %d is thinking\n", philo, semaphores);
	sem_wait(semaphores->forks);
	semlock_printf("%lld %d has taken a fork\n", philo, semaphores);
	if (is_death_flag(philo, semaphores) || philo->total_philo == 1)
	{
		sem_post(semaphores->forks);
		return (ERROR);
	}
	sem_wait(semaphores->forks);
	semlock_printf("%lld %d has taken a fork\n", philo, semaphores);
	if (is_death_flag(philo, semaphores) || philo->total_philo == 1)
	{
		sem_post(semaphores->forks);
		sem_post(semaphores->forks);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	start_eating(t_philo *philo, t_sem *semaphores)
{
	semlock_gettimeofday(&philo->started_eat, semaphores->time);
	semlock_printf("%lld %d is eating\n", philo, semaphores);
	while (1)
	{
		if (is_death_flag(philo, semaphores))
		{
			sem_post(semaphores->forks);
			sem_post(semaphores->forks);
			return (ERROR);
		}
		if (curr_timestamp(philo->started_eat,
				semaphores->time) >= philo->tt_eat)
			break ;
		usleep(250);
	}
	sem_post(semaphores->forks);
	sem_post(semaphores->forks);
	sem_wait(semaphores->meals_eaten);
	philo->meals_eaten++;
	sem_post(semaphores->meals_eaten);
	return (SUCCESS);
}

static int	start_sleeping(t_philo *philo, t_sem *semaphores)
{
	semlock_gettimeofday(&philo->started_sleep, semaphores->time);
	semlock_printf("%lld %d is sleeping\n", philo, semaphores);
	while (1)
	{
		if (is_death_flag(philo, semaphores))
			return (ERROR);
		if (curr_timestamp(philo->started_sleep,
				semaphores->time) >= philo->tt_sleep)
			break ;
		usleep(250);
	}
	return (SUCCESS);
}

static void	terminate_if_eat_max(t_philo *philo,
		t_sem *semaphores, pthread_t thread_1, pthread_t thread_2)
{
	sem_wait(semaphores->meals_eaten);
	if (philo->eat_max != -1
		&& philo->meals_eaten == philo->eat_max)
	{
		sem_post(semaphores->meals_eaten);
		usleep((philo->tt_eat + philo->tt_sleep) * 1000);
		sem_wait(semaphores->death_flag);
		philo->death_flag = 1;
		sem_post(semaphores->death_flag);
		sem_post(semaphores->death_occurred);
		subprocess_close_sem_exit(semaphores, thread_1, thread_2);
	}
	else
		sem_post(semaphores->meals_eaten);
}

void	philo_child_routine(t_philo *philo, t_sem *semaphores)
{
	pthread_t		d_tracker;
	pthread_t		d_setter;
	t_threads_args	args;

	args.philo = philo;
	args.semaphores = semaphores;
	philo->started_eat = philo->start_time;
	create_threads(&args, &d_tracker, &d_setter);
	if (is_even(philo->philo_num))
		if (start_sleeping(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, d_tracker, d_setter);
	while (1)
	{
		terminate_if_eat_max(philo, semaphores, d_tracker, d_setter);
		if (start_thinking(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, d_tracker, d_setter);
		if (start_eating(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, d_tracker, d_setter);
		terminate_if_eat_max(philo, semaphores, d_tracker, d_setter);
		if (start_sleeping(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, d_tracker, d_setter);
	}
	subprocess_close_sem_exit(semaphores, d_tracker, d_setter);
}
