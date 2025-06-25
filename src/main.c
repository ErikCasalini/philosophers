/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/25 10:48:39 by ecasalin         ###   ########.fr       */
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
		// pthread_mutex_lock(args->philo->print_mutex);
		// pthread_mutex_unlock(args->philo->print_mutex);
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
		// pthread_mutex_lock(args->philo->print_mutex);
		mutex_printf("%lld %d has taken a fork\n", args);
		// pthread_mutex_unlock(args->philo->print_mutex);
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
		// pthread_mutex_lock(args->philo->print_mutex);
		mutex_printf("%lld %d has taken a fork\n", args);
		// pthread_mutex_unlock(args->philo->print_mutex);
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
	// printf("%lld %d dropped forks\n",
	// 	curr_timestamp(args->philo->start_time), args->philo_num);
	return (SUCCESS);
}

int	start_thinking(t_thread_args *args)
{
	int	holded_forks[2];

	holded_forks[0] = 0;
	holded_forks[1] = 0;
	// pthread_mutex_lock(args->philo->print_mutex);
	mutex_printf("%lld %d is thinking\n", args);
	// pthread_mutex_unlock(args->philo->print_mutex);
	while (!holded_forks[0] || !holded_forks[1])
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (args->philo_num % 2 == 0)
		{
			// usleep(100000);
			if (!holded_forks[1])
				try_right_fork(args, &holded_forks[1]);
			// usleep(1000);
			if (!holded_forks[0])
				try_left_fork(args, &holded_forks[0]);
			// usleep(1000);
		}
		else
		{
			// usleep(50000);
			if (!holded_forks[0])
				try_left_fork(args, &holded_forks[0]);
			// usleep(1000);
			if (!holded_forks[1])
				try_right_fork(args, &holded_forks[1]);
			// usleep(1000);
		}
	}
	return (SUCCESS);
}

int	start_eating(t_thread_args *args)
{
	gettimeofday(&args->started_eat, NULL);
	// pthread_mutex_lock(args->philo->print_mutex);
	mutex_printf("%lld %d is eating\n", args);
	// pthread_mutex_unlock(args->philo->print_mutex);
	while (1)
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (curr_timestamp(args->started_eat) >= args->philo->tt_eat)
			break ;
		usleep(10);
	}
	drop_forks(args);
	args->meals_eaten++;
	return (SUCCESS);
}

int	start_sleeping(t_thread_args *args)
{
	gettimeofday(&args->started_sleep, NULL);
	// pthread_mutex_lock(args->philo->print_mutex);
	mutex_printf("%lld %d is sleeping\n", args);
	// pthread_mutex_unlock(args->philo->print_mutex);
	while (1)
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (curr_timestamp(args->started_sleep) >= args->philo->tt_sleep)
			break ;
		usleep(10);
	}
	return (SUCCESS);
}

void	*routine(void *args_struct)
{
	t_thread_args *args;

	args = (t_thread_args *)args_struct;
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
	// printf("p_num:%d, my forks are left=%d and right=%d, ttd: %d, tte: %d, tts: %d, eat_max: %d\n", args->philo_num, args->side_forks.left, args->side_forks.right, args->philo.tt_die, args->philo.tt_eat, args->philo.tt_sleep, args->philo.eat_max);
	return (NULL);
}

int	create_threads(t_philo *philo, int total_philo, t_heap_allocated *heap)
{
	int				philo_num;
	t_side_forks	side_forks;

	philo_num = 0;
	while (philo_num < total_philo)
	{
		side_forks = set_forks(philo_num, total_philo);
		heap->thread_args[philo_num] = (t_thread_args)
		{
			philo,
			philo_num + 1,
			side_forks,
			philo->start_time,
			(struct timeval){0, 0},
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
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_philo				philo;
	t_intf				total_philo;
	t_heap_allocated	heap;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		print_mutex;
	
	if (argc < 5 || argc > 6)
		exit_bad_argument();
		
	heap = (t_heap_allocated){NULL, NULL, NULL, NULL};
	
	total_philo = ft_atoi_flag(argv[1]); // --> HAVE TO FIX ATOI : -> r1234 should not work //
	if (total_philo.flag == ERROR)
		exit_bad_argument();
		
	if (init_philo_struct(argc, argv, &philo, &death_mutex, &print_mutex) == ERROR)
		exit_bad_argument();
	if (allocate_heap(&heap, total_philo.value) == ERROR)
		free_heap_exit_err(&heap);
		
	init_mutexes(heap.mutexes, total_philo.value, &death_mutex, &print_mutex);
	create_threads(&philo, total_philo.value, &heap);
	
	join_threads(heap.thread_lst, total_philo.value);
	destroy_mutexes(heap.mutexes, total_philo.value, &death_mutex, &print_mutex);
	free_heap(&heap);
	return (0);
}
