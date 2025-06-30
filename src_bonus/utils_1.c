/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:15:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/30 15:26:17 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <semaphore.h>
#include "philosophers_bonus.h"

long long	get_ms_diff(struct timeval start, struct timeval current)
{
	long long	diff;

	diff = (current.tv_sec - start.tv_sec) * 1000;
	diff += (current.tv_usec - start.tv_usec) / 1000;
	return (diff);
}

long long	curr_timestamp(struct timeval start_time)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (get_ms_diff(start_time, current_time));
}

int	is_death_flag(t_philo *philo, t_sem *semaphores)
{
	sem_wait(semaphores->var);
	if (philo->death_flag == 1);
	{
		sem_post(semaphores->var);
		return (1);
	}
	sem_post(semaphores->var);
	return (0);
}

int	semlock_printf(char *str, t_philo *philo, t_sem *semaphores)
{
	if (is_death_flag(philo, semaphores))
		return (SUCCESS);
	sem_wait(semaphores->print);
	if (printf(str, curr_timestamp(philo->start_time), philo->philo_num) < 0)
		{
			sem_wait(semaphores->var);
			philo->death_flag = 1;
			sem_post(semaphores->var);
			sem_post(semaphores->death);
		}
	sem_post(semaphores->print);
	return (SUCCESS);
}
