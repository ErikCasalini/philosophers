/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/26 15:49:21 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "philosophers.h"
#include <stdlib.h>
#include <string.h>

int	everybody_lives(t_thread_args *args)
{
	int				ms_diff;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	ms_diff = get_ms_diff(args->started_eat, current_time);
	pthread_mutex_lock(args->philo->death_mutex);
	if (args->philo->death_flag)
	{
		pthread_mutex_unlock(args->philo->death_mutex);
		return (0);
	}
	if (ms_diff >= args->philo->tt_die)
	{
		args->philo->death_flag = 1;
		printf("%lld %d died\n",
			curr_timestamp(args->philo->start_time), args->philo_num);
		pthread_mutex_unlock(args->philo->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(args->philo->death_mutex);
	return (1);
}

int	try_left_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(&args->mutexes[args->side_forks.left]);
	if (args->forks[args->side_forks.left])
	{
		args->forks[args->side_forks.left] = 0;
		*holded_forks += 1;
		mutex_printf("%lld %d has taken a fork\n", args);
	}
	pthread_mutex_unlock(&args->mutexes[args->side_forks.left]);
	return (SUCCESS);
}

int	try_right_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(&args->mutexes[args->side_forks.right]);
	if (args->forks[args->side_forks.right])
	{
		args->forks[args->side_forks.right] = 0;
		*holded_forks += 1;
		mutex_printf("%lld %d has taken a fork\n", args);
	}
	pthread_mutex_unlock(&args->mutexes[args->side_forks.right]);
	return (SUCCESS);
}

int	drop_forks(t_thread_args *args)
{
	pthread_mutex_lock(&args->mutexes[args->side_forks.left]);
	args->forks[args->side_forks.left] = 1;
	pthread_mutex_unlock(&args->mutexes[args->side_forks.left]);
	pthread_mutex_lock(&args->mutexes[args->side_forks.right]);
	args->forks[args->side_forks.right] = 1;
	pthread_mutex_unlock(&args->mutexes[args->side_forks.right]);
	gettimeofday(&args->started_sleep, NULL);
	return (SUCCESS);
}

int	wait_your_turn(t_thread_args *args)
{
	gettimeofday(&args->started_think, NULL);
	while (1)
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (curr_timestamp(args->started_think) >= args->philo->tt_think)
			break ;
		usleep(500);
	}
	return (SUCCESS);
}

int	is_last_to_eat(t_thread_args *args, int last_to_eat_tracker)
{
	if (args->philo_num + last_to_eat_tracker == args->philo->total_philo)
		return (1);
	return (0);
}

int	start_thinking(t_thread_args *args, int last_to_eat_tracker)
{
	int	holded_forks[2];

	holded_forks[0] = 0;
	holded_forks[1] = 0;
	mutex_printf("%lld %d is thinking\n", args);
	if (last_to_eat_tracker % 2 == 0)
	{
		if (args->philo->tt_think != 0
			&& args->philo_num % 2 == 1
			|| (is_last_to_eat(args, last_to_eat_tracker) && args->philo->total_philo % 2 == 1))
			if (wait_your_turn(args) == ERROR)
				return (ERROR);
	}
	else if (last_to_eat_tracker % 2 == 1)
	{
		if (args->philo->tt_think != 0
			&& args->philo_num % 2 == 0
			|| (is_last_to_eat(args, last_to_eat_tracker) && args->philo->total_philo % 2 == 1))
			if (wait_your_turn(args) == ERROR)
				return (ERROR);
	}
	while (!holded_forks[0] || !holded_forks[1])
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (!holded_forks[0])
			try_left_fork(args, &holded_forks[0]);
		if (!holded_forks[1])
			try_right_fork(args, &holded_forks[1]);
		usleep(500);
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
		usleep(500);
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
		usleep(500);
	}
	return (SUCCESS);
}

void	*routine(void *args_struct)
{
	int				last_to_eat_tracker;
	t_thread_args	*args;

	args = (t_thread_args *)args_struct;
	pthread_mutex_lock(args->philo->sync_mutex);
	pthread_mutex_unlock(args->philo->sync_mutex);
	last_to_eat_tracker = 0;
	args->started_eat = args->philo->start_time;
	if (args->philo->tt_think == 0)
	{
		if (args->philo_num % 2 == 0 || (is_last_to_eat(args, last_to_eat_tracker) && args->philo->total_philo % 2 == 1))
			if (start_sleeping(args) == ERROR)
				return (NULL);
		last_to_eat_tracker++;
	}
	while (1)
	{
		if (args->philo->eat_max != -1
				&& args->meals_eaten == args->philo->eat_max)
			return (NULL);
		if (start_thinking(args, last_to_eat_tracker) == ERROR)
			return (NULL);
		if (start_eating(args) == ERROR)
			return (NULL);
		if (start_sleeping(args) == ERROR)
			return (NULL);
		last_to_eat_tracker++;
		if (last_to_eat_tracker == args->philo->total_philo)
			last_to_eat_tracker = 0;
	}
	return (NULL);
}

int	create_threads(t_philo *philo, int total_philo, t_heap_allocated *heap)
{
	int				philo_num;
	t_side_forks	side_forks;

	philo_num = 0;
	pthread_mutex_lock(philo->sync_mutex);
	while (philo_num < total_philo)
	{
		side_forks = set_forks(philo_num, total_philo);
		heap->thread_args[philo_num] = (t_thread_args)
		{
			philo,
			philo_num + 1,
			side_forks,
			(struct timeval){0, 0},
			(struct timeval){0, 0},
			(struct timeval){0, 0},
			0,
			0,
			heap->forks,
			heap->mutexes
		};
		pthread_create(&heap->thread_lst[philo_num],
			NULL,
			routine,
			&heap->thread_args[philo_num]);
		philo_num++;
	}
	gettimeofday(&philo->start_time, NULL);
	pthread_mutex_unlock(philo->sync_mutex);
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_philo				philo;
	t_intf				total_philo;
	t_heap_allocated	heap;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		sync_mutex;

	if (argc < 5 || argc > 6)
		exit_bad_argument();

	heap = (t_heap_allocated){NULL, NULL, NULL, NULL};

	total_philo = ft_atoi_flag(argv[1]); // --> HAVE TO FIX ATOI : -> r1234 should not work //
	if (total_philo.flag == ERROR)
		exit_bad_argument();

	philo.total_philo = total_philo.value;
	if (init_philo_struct(argc, argv, &philo, &death_mutex, &sync_mutex) == ERROR)
		exit_bad_argument();
	if (allocate_heap(&heap, total_philo.value) == ERROR)
		free_heap_exit_err(&heap);

	init_mutexes(heap.mutexes, total_philo.value, &death_mutex, &sync_mutex);
	create_threads(&philo, total_philo.value, &heap);

	join_threads(heap.thread_lst, total_philo.value);
	destroy_mutexes(heap.mutexes, total_philo.value, &death_mutex, &sync_mutex);
	free_heap(&heap);
	return (0);
}
