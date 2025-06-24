/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/24 08:54:14 by ecasalin         ###   ########.fr       */
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

void	*routine(void *args_struct)
{
	t_thread_args *args;

	args = (t_thread_args *)args_struct;
	printf("p_num:%d, my forks are left=%d and right=%d, ttd: %d, tte: %d, tts: %d, eat_max: %d\n", args->philo_num, args->side_forks.left, args->side_forks.right, args->philo.tt_die, args->philo.tt_eat, args->philo.tt_sleep, args->philo.eat_max);
	return (NULL);
}

int	create_threads(t_philo *philo, int philo_num, t_heap_allocated *heap)
{
	int		current_philo;
	t_side_forks side_forks;

	current_philo = 0;
	while (current_philo < philo_num)
	{
		side_forks = set_forks(current_philo, philo_num);
		heap->thread_args[current_philo] = (t_thread_args){*philo, current_philo + 1, side_forks};
		pthread_create(&heap->thread_lst[current_philo], NULL, routine, &heap->thread_args[current_philo]);
		current_philo++;
	}
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_philo				philo;
	t_intf				philo_num;
	t_heap_allocated	heap;
	/* HAVE TO FIX ATOI : -> r1234 should not work */
	if (argc < 5 || argc > 6)
		exit_bad_argument();
	heap = (t_heap_allocated){NULL, NULL, NULL};
	philo_num = ft_atoi_flag(argv[1]);
	if (philo_num.flag == ERROR)
		exit_bad_argument();
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	if (allocate_heap(&heap, philo_num.value) == ERROR)
		free_heap_exit_err(&heap);
	create_threads(&philo, philo_num.value, &heap);
	pthread_join(heap.thread_lst[0], NULL);
	pthread_join(heap.thread_lst[1], NULL);
	pthread_join(heap.thread_lst[2], NULL);
	free_heap(&heap);
	return (0);
}
