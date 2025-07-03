/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/02 14:12:24 by ecasalin         ###   ########.fr       */
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
	ft_putstr_fd(err_msg, 2);
	pthread_mutex_unlock(death_mutex);
	philo->return_value = ERROR;
	return (return_value);
}

int	create_threads(t_philo *philo,
		t_mutexes *mutexes, int total_philo, t_heap_allocated *heap)
{
	int				philo_num;

	if (gettimeofday(&philo->start_time, NULL) != SUCCESS)
		return (print_error_set_flag("Warning: unable to get time of day\n",
				0, philo, &mutexes->death_mutex));
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
	if (philo.eat_max == 0)
		return (0);
	if (allocate_heap(&heap, total_philo.value) == ERROR)
		free_heap_exit_err(&heap);
	init_mutexes(&mutexes, heap.fork_mutexes, total_philo.value);
	join_threads(heap.thread_lst,
		create_threads(&philo, &mutexes, total_philo.value, &heap));
	destroy_mutexes(&mutexes, total_philo.value);
	free_heap(&heap);
	return (philo.return_value);
}
