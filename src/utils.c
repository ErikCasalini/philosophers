/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:35:35 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/25 10:49:34 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>

long long	get_ms_diff(struct timeval start, struct timeval current)
{
	long long	diff;

	diff = (current.tv_sec - start.tv_sec) * 1000;
	diff += (current.tv_usec - start.tv_usec) / 1000;
	return (diff);
}

long long	curr_timestamp(struct timeval start_time)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	return (get_ms_diff(start_time, current_time));
}

void	mutex_printf(char *str, t_thread_args *args)
{
	pthread_mutex_lock(args->philo->death_mutex);
	if (!args->philo->death_flag)
	{
		// pthread_mutex_lock(args->philo->print_mutex);
		printf(str, curr_timestamp(args->philo->start_time), args->philo_num);
		// pthread_mutex_unlock(args->philo->print_mutex);
	}
	pthread_mutex_unlock(args->philo->death_mutex);
}