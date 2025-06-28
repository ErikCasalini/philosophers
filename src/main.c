/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 10:13:50 by ecasalin         ###   ########.fr       */
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

int	try_left_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	if (args->forks[args->side_forks.left])
	{
		args->forks[args->side_forks.left] = 0;
		*holded_forks += 1;
		mutex_printf("%lld %d has taken a fork\n", args);
	}
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	return (SUCCESS);
}

int	try_right_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.right]);
	if (args->forks[args->side_forks.right])
	{
		args->forks[args->side_forks.right] = 0;
		*holded_forks += 1;
		mutex_printf("%lld %d has taken a fork\n", args);
	}
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.right]);
	return (SUCCESS);
}

int	drop_forks(t_thread_args *args)
{
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	args->forks[args->side_forks.left] = 1;
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.right]);
	args->forks[args->side_forks.right] = 1;
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.right]);
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
		usleep(250);
	}
	return (SUCCESS);
}

int	is_last_to_eat(t_thread_args *args, int last_to_eat_tracker)
{
	if (args->philo_num + last_to_eat_tracker == args->philo->total_philo)
		return (1);
	return (0);
}

int	start_thinking(t_thread_args *args, int	*odd_even_turns)
{
	int	holded_forks[2];

	holded_forks[0] = 0;
	holded_forks[1] = 0;
	mutex_printf("%lld %d is thinking\n", args);
	// if (is_even(*odd_even_turns))
	// {
	// 	if (args->philo->tt_think != 0
	// 		&& !is_even(args->philo_num))
	// 		if (wait_your_turn(args) == ERROR)
	// 			return (ERROR);
	// }
	// else if (!is_even(*odd_even_turns))
	// {
	// 	if (args->philo->tt_think != 0
	// 		&& is_even(args->philo_num))
	// 		if (wait_your_turn(args) == ERROR)
	// 			return (ERROR);
	// }
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
	int				odd_even_turns;

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
		if (start_thinking(args, &odd_even_turns) == ERROR)
			return (NULL);
		if (start_eating(args) == ERROR)
			return (NULL);
		if (start_sleeping(args) == ERROR)
			return (NULL);
		odd_even_turns++;
	}
	return (NULL);
}

int	create_threads(t_philo *philo, t_mutexes *mutexes, int total_philo, t_heap_allocated *heap)
{
	int				philo_num;

	philo_num = 0;
	pthread_mutex_lock(&mutexes->sync_mutex);
	while (philo_num < total_philo)
	{
		heap->thread_args[philo_num].mutexes = mutexes;
		init_thread_args_struct(heap, philo, philo_num, total_philo);
		if (pthread_create(&heap->thread_lst[philo_num],
			NULL,
			routine,
			&heap->thread_args[philo_num]) > SUCCESS)
		{
			pthread_mutex_lock(&mutexes->death_mutex);
			philo->death_flag = 1;
			pthread_mutex_unlock(&mutexes->death_mutex);
			break ;
		}
		philo_num++;
	}
	gettimeofday(&philo->start_time, NULL);
	pthread_mutex_unlock(&mutexes->sync_mutex);
	return (philo_num);
}

int	main(int argc, char *argv[])
{
	t_philo				philo;
	t_intf				total_philo;
	t_heap_allocated	heap;
	t_mutexes			mutexes;

	if (argc < 5 || argc > 6)
		exit_bad_argument();
	heap = (t_heap_allocated){NULL, NULL, NULL, NULL};
	total_philo = ft_atoi_flag(argv[1]);
	if (total_philo.flag == ERROR)
		exit_bad_argument();
	philo.total_philo = total_philo.value;
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	if (allocate_heap(&heap, total_philo.value) == ERROR)
		free_heap_exit_err(&heap);
	init_mutexes(&mutexes, heap.fork_mutexes, total_philo.value);
	join_threads(heap.thread_lst,
			create_threads(&philo, &mutexes, total_philo.value, &heap));
	destroy_mutexes(&mutexes, total_philo.value);
	free_heap(&heap);
	return (0);
}
