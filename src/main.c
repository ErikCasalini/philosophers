/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/24 12:00:06 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "philosophers.h"
#include <stdlib.h>
#include <string.h>

// int	main(void)
// {
	// struct timeval starting_time;
	// struct timeval curr_time;
	// int	i = 0;
//
	// gettimeofday(&starting_time, NULL);
	// while(i++ <= 20)
	// {
		// usleep(1000000);
		// gettimeofday(&curr_time, NULL);
		// printf("Diff: %lld\n", get_ms_diff(starting_time, curr_time));
	// }
// }

// int	random_time()
// {
// 	srand((unsigned int)time(NULL));
// 	return ((rand() % (8000000 - 1000000 + 1) + 1000000));
// }


// void	*routine(void *args)
// {
// 	t_philo *casted_args;
// 	struct timeval curr_time;
// 	int	ms_diff;

// 	casted_args = (t_philo*)args;
// 	gettimeofday(&curr_time, NULL);
// 	printf("%d : I'm born at %lld ms from program launch\n" , casted_args->philo_num, get_ms_diff(casted_args->start_time, curr_time));
// 	usleep(casted_args->rand_num);
// 	while(1)
// 	{
// 		gettimeofday(&curr_time, NULL);
// 		ms_diff = get_ms_diff(casted_args->start_time, curr_time);
// 		if (ms_diff >= casted_args->tt_die)
// 		{
// 			printf("%d : I died at %d ms after program starting\n", casted_args->philo_num, ms_diff);
// 			return (NULL);
// 		}
// 	}
// 	return (NULL);
// }

// int	main(void)
// {
// 	pthread_t t1;
// 	pthread_t t2;
// 	struct timeval starting_time;
// 	t_philo args1;
// 	t_philo args2;

// 	gettimeofday(&starting_time, NULL);
// 	args1 = (t_philo){1, starting_time, 400, random_time()};
// 	args2 = (t_philo){2, starting_time, 300, random_time()};
// 	// usleep(1000000);
// 	pthread_create(&t1, NULL, routine, &args1);
// 	// usleep(1000000);
// 	pthread_create(&t2, NULL, routine, &args2);
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// }

// int	main(void)
// {
	// 	int i = random_time();
	// 	printf("%d\n", i);
	// 	usleep(i);
	// }

int	everybody_lives(t_thread_args *args)
{
	int	ms_diff;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	ms_diff = get_ms_diff(args->started_eat, current_time);
	pthread_mutex_lock(args->mutex);
	if (args->philo.death_flag)
	{
		pthread_mutex_unlock(args->mutex);
		return (0);
	}
	if (ms_diff >= args->philo.tt_die)
	{
		args->philo.death_flag = 1;
		pthread_mutex_unlock(args->mutex);
		printf("%lld %d died\n",
			curr_timestamp(args->philo.start_time), args->philo_num);
		return (0);
	}
	pthread_mutex_unlock(args->mutex);
	return (1);
}

int	try_left_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(args->mutex);
	if (args->side_forks.left)
	{
		args->side_forks.left = 0;
		*holded_forks += 1;
		printf("%lld %d has taken a fork\n",
			curr_timestamp(args->philo.start_time), args->philo_num);
	}
	pthread_mutex_unlock(args->mutex);
	return (SUCCESS);
}

int	try_right_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(args->mutex);
	if (args->side_forks.right)
	{
		args->side_forks.right = 0;
		*holded_forks += 1;
		printf("%lld %d has taken a fork\n",
			curr_timestamp(args->philo.start_time), args->philo_num);
	}
	pthread_mutex_unlock(args->mutex);
	return (SUCCESS);
}

int	drop_forks(t_thread_args *args)
{
	pthread_mutex_lock(args->mutex);
	args->side_forks.left = 1;
	args->side_forks.right = 1;
	pthread_mutex_unlock(args->mutex);
	return (SUCCESS);
}

int	start_thinking(t_thread_args *args)
{
	int	holded_forks[2];

	holded_forks[0] = 0;
	holded_forks[1] = 0;
	printf("%lld %d is thinking\n",
			curr_timestamp(args->philo.start_time), args->philo_num);
	while (!holded_forks[0] || !holded_forks[1])
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (args->philo_num % 2 == 0)
		{
			if (!holded_forks[1])
				try_right_fork(args, &holded_forks[1]);
			if (!holded_forks[0])
				try_left_fork(args, &holded_forks[0]);
		}
		else
		{
			if (!holded_forks[0])
				try_left_fork(args, &holded_forks[0]);
			if (!holded_forks[1])
				try_right_fork(args, &holded_forks[1]);
		}
	}
	return (SUCCESS);
}

int	start_eating(t_thread_args *args)
{
	gettimeofday(&args->started_eat, NULL);
	printf("%lld %d is eating\n",
		curr_timestamp(args->philo.start_time), args->philo_num);
	while (1)
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (curr_timestamp(args->started_eat) >= args->philo.tt_eat)
			break ;
		usleep(10);
	}
	drop_forks(args);
	return (SUCCESS);
}

int	start_sleeping(t_thread_args *args)
{
	gettimeofday(&args->started_sleep, NULL);
	printf("%lld %d is sleeping\n",
		curr_timestamp(args->philo.start_time), args->philo_num);
	while (1)
	{
		if (!everybody_lives(args))
			return (ERROR);
		if (curr_timestamp(args->started_sleep) >= args->philo.tt_sleep)
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

int	join_threads(pthread_t *thread_lst, int total_philo)
{
	int	i;

	i = 0;
	while (i < total_philo)
	{
		pthread_join(thread_lst[i], NULL);
		i++;
	}
	return (SUCCESS);
}

int	create_threads(t_philo *philo, int total_philo, t_heap_allocated *heap, pthread_mutex_t *mutex)
{
	int				philo_num;
	t_side_forks	side_forks;

	philo_num = 0;
	while (philo_num < total_philo)
	{
		side_forks = set_forks(philo_num, total_philo);
		heap->thread_args[philo_num] = (t_thread_args)
		{
			*philo,
			philo_num + 1,
			side_forks,
			philo->start_time,
			(struct timeval){0},
			mutex
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
	pthread_mutex_t		mutex;
	/* HAVE TO FIX ATOI : -> r1234 should not work */
	if (argc < 5 || argc > 6)
		exit_bad_argument();
	heap = (t_heap_allocated){NULL, NULL, NULL};
	total_philo = ft_atoi_flag(argv[1]);
	if (total_philo.flag == ERROR)
		exit_bad_argument();
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	if (allocate_heap(&heap, total_philo.value) == ERROR)
		free_heap_exit_err(&heap);
	pthread_mutex_init(&mutex, NULL);
	create_threads(&philo, total_philo.value, &heap, &mutex);
	join_threads(heap.thread_lst, total_philo.value);
	pthread_mutex_destroy(&mutex);
	free_heap(&heap);
	return (0);
}
