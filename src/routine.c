/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:05:12 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 11:52:59 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "philosophers.h"

int	everybody_lives(t_thread_args *args)
{
	int				ms_diff;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	ms_diff = get_ms_diff(args->started_eat, current_time);
	pthread_mutex_lock(&args->mutexes->death_mutex);
	if (args->philo->death_flag)
	{
		pthread_mutex_unlock(&args->mutexes->death_mutex);
		return (0);
	}
	if (ms_diff >= args->philo->tt_die)
	{
		args->philo->death_flag = 1;
		printf("%lld %d died\n",
			curr_timestamp(args->philo->start_time), args->philo_num);
		pthread_mutex_unlock(&args->mutexes->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&args->mutexes->death_mutex);
	return (1);
}

int	start_thinking(t_thread_args *args)
{
	int	holded_forks[2];

	holded_forks[0] = 0;
	holded_forks[1] = 0;
	mutex_printf("%lld %d is thinking\n", args);
	while (!holded_forks[0] || !holded_forks[1])
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (!holded_forks[0])
			try_left_fork(args, &holded_forks[0]);
		if (!holded_forks[1])
			try_right_fork(args, &holded_forks[1]);
		usleep(250);
	}
	return (SUCCESS);
}

int	start_eating(t_thread_args *args)
{
	gettimeofday(&args->started_eat, NULL);
	mutex_printf("%lld %d is eating\n", args);
	while (1)
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (curr_timestamp(args->started_eat) >= args->philo->tt_eat)
			break ;
		usleep(250);
	}
	drop_forks(args);
	args->meals_eaten++;
	return (SUCCESS);
}

int	start_sleeping(t_thread_args *args)
{
	gettimeofday(&args->started_sleep, NULL);
	mutex_printf("%lld %d is sleeping\n", args);
	while (1)
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (curr_timestamp(args->started_sleep) >= args->philo->tt_sleep)
			break ;
		usleep(250);
	}
	return (SUCCESS);
}

void	*routine(void *args_struct)
{
	t_thread_args	*args;

	args = (t_thread_args *)args_struct;
	pthread_mutex_lock(&args->mutexes->sync_mutex);
	pthread_mutex_unlock(&args->mutexes->sync_mutex);
	args->started_eat = args->philo->start_time;
	if (is_even(args->philo_num))
		if (start_sleeping(args) == ERROR)
			return (NULL);
	while (1)
	{
		if (args->philo->eat_max != -1
			&& args->meals_eaten == args->philo->eat_max)
			return (NULL);
		if (start_thinking(args) == ERROR)
			return (NULL);
		if (start_eating(args) == ERROR)
			return (NULL);
		if (start_sleeping(args) == ERROR)
			return (NULL);
	}
	return (NULL);
}
