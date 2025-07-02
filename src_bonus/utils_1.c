/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:15:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/02 09:34:23 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include "philosophers_bonus.h"

long long	get_ms_diff(struct timeval start, struct timeval current)
{
	long long	diff;

	diff = (current.tv_sec - start.tv_sec) * 1000;
	diff += (current.tv_usec - start.tv_usec) / 1000;
	return (diff);
}

long long	curr_timestamp(struct timeval start_time, sem_t *time)
{
	struct timeval	current_time;

	semlock_gettimeofday(&current_time, time);
	return (get_ms_diff(start_time, current_time));
}

int	is_death_flag(t_philo *philo, t_sem *semaphores)
{
	sem_wait(semaphores->death_flag);
	if (philo->death_flag == 1)
	{
		sem_post(semaphores->death_flag);
		return (1);
	}
	sem_post(semaphores->death_flag);
	return (0);
}

int	semlock_printf(char *str, t_philo *philo, t_sem *semaphores)
{
	sem_wait(semaphores->print);
	if (is_death_flag(philo, semaphores))
	{
		sem_post(semaphores->print);
		return (SUCCESS);
	}
	if (printf(str, curr_timestamp(philo->start_time, semaphores->time), philo->philo_num) < 0)
		{
			sem_wait(semaphores->death_flag);
			philo->death_flag = 1;
			sem_post(semaphores->death_flag);
			sem_post(semaphores->death_occurred);
		}
	sem_post(semaphores->print);
	return (SUCCESS);
}

void	semlock_gettimeofday(struct timeval *time_buffer, sem_t *time)
{
	sem_wait(time);
	gettimeofday(time_buffer, NULL);
	sem_post(time);
}
