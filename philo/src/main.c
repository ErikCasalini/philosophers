/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/04 08:20:27 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "philosophers.h"
#include <stdlib.h>
#include <string.h>

static int	print_error_set_flag(char *err_msg,
		int return_value, t_philo *philo, pthread_mutex_t *death_mutex)
{
	pthread_mutex_lock(death_mutex);
	philo->death_flag = 1;
	pthread_mutex_unlock(death_mutex);
	ft_putstr_fd(err_msg, 2);
	philo->return_value = ERROR;
	return (return_value);
}

static void	print_error_exit(char *err_msg)
{
	ft_putstr_fd(err_msg, 2);
	exit(EXIT_FAILURE);
}

int	create_threads(t_philo *philo,
		t_mutexes *mutexes, t_heap_allocated *heap)
{
	int				philo_num;

	philo_num = 0;
	pthread_mutex_lock(&mutexes->sync_mutex);
	while (philo_num < philo->total_philo)
	{
		init_thread_args_struct(heap, philo, philo_num, mutexes);
		if (pthread_create(&heap->thread_lst[philo_num],
				NULL,
				routine,
				&heap->thread_args[philo_num]) > SUCCESS)
		{
			print_error_set_flag("Thread creation error\n",
				0, philo, &mutexes->death_mutex);
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
	t_heap_allocated	heap;
	t_mutexes			mutexes;

	if (argc < 5 || argc > 6)
		exit_bad_argument();
	heap = (t_heap_allocated){NULL, NULL, NULL, NULL};
	if (gettimeofday(&philo.start_time, NULL) != SUCCESS)
		print_error_exit("Unable to get time\n");
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	if (philo.eat_max == 0)
		return (EXIT_SUCCESS);
	if (allocate_heap(&heap, philo.total_philo) == ERROR)
		free_heap_exit_err("Memory allocation error\n", &heap);
	if (init_mutexes(&mutexes, heap.fork_mutexes, philo.total_philo) == ERROR)
		free_heap_exit_err("Mutexes init error\n", &heap);
	join_threads(heap.thread_lst,
		create_threads(&philo, &mutexes, &heap));
	destroy_mutexes(&mutexes, philo.total_philo);
	free_heap(&heap);
	return (philo.return_value);
}
