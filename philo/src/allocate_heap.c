/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_heap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:28:06 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 11:47:05 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <string.h>

int	allocate_heap(t_heap_allocated *heap, int total_philo)
{
	heap->forks = alloc_forks_array(total_philo);
	if (heap->forks == NULL)
		return (ERROR);
	heap->thread_lst = alloc_thread_lst(total_philo);
	if (heap->thread_lst == NULL)
		return (ERROR);
	heap->thread_args = alloc_thread_args(total_philo);
	if (heap->thread_args == NULL)
		return (ERROR);
	heap->fork_mutexes = alloc_fork_mutexes(total_philo);
	if (heap->fork_mutexes == NULL)
		return (ERROR);
	return (SUCCESS);
}

char	*alloc_forks_array(int fork_num)
{
	char	*forks;

	forks = malloc(fork_num);
	if (forks == NULL)
		return (NULL);
	memset(forks, 1, fork_num);
	return (forks);
}

pthread_mutex_t	*alloc_fork_mutexes(int mutexes_num)
{
	pthread_mutex_t	*fork_mutexes;

	fork_mutexes = malloc(sizeof(pthread_mutex_t) * mutexes_num);
	if (fork_mutexes == NULL)
		return (NULL);
	memset(fork_mutexes, 0, sizeof(pthread_mutex_t) * mutexes_num);
	return (fork_mutexes);
}

pthread_t	*alloc_thread_lst(int thread_num)
{
	pthread_t	*thread_lst;

	thread_lst = malloc(sizeof(pthread_t) * thread_num);
	if (thread_lst == NULL)
		return (NULL);
	memset(thread_lst, 0, sizeof(pthread_t) * thread_num);
	return (thread_lst);
}

t_thread_args	*alloc_thread_args(int thread_num)
{
	t_thread_args	*thread_args;

	thread_args = malloc(sizeof(t_thread_args) * thread_num);
	if (thread_args == NULL)
		return (NULL);
	memset(thread_args, 0, sizeof(t_thread_args) * thread_num);
	return (thread_args);
}
