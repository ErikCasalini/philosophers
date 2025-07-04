/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init_tools_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 08:05:41 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/04 08:11:06 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_thread_args_struct(t_heap_allocated *heap,
			t_philo *philo, int philo_num, t_mutexes *mutexes)
{
	t_side_forks	side_forks;

	side_forks = init_forks_indexes(philo_num, philo->total_philo);
	heap->thread_args[philo_num].philo = philo;
	heap->thread_args[philo_num].mutexes = mutexes;
	heap->thread_args[philo_num].forks = heap->forks;
	heap->thread_args[philo_num].philo_num = philo_num + 1;
	heap->thread_args[philo_num].side_forks = side_forks;
	heap->thread_args[philo_num].started_think = (struct timeval){0, 0};
	heap->thread_args[philo_num].started_eat = (struct timeval){0, 0};
	heap->thread_args[philo_num].started_sleep = (struct timeval){0, 0};
	heap->thread_args[philo_num].meals_eaten = 0;
}
