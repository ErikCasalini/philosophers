/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init_tools_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:47:58 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/02 14:03:08 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <fcntl.h>
#include <semaphore.h>
#include <sys/time.h>

static int	check_and_set_arg(char *arg, int *philo_param)
{
	t_intf	temp_atoi;

	temp_atoi = ft_atoi_flag(arg);
	if (temp_atoi.flag)
		return (ERROR);
	else
		*philo_param = temp_atoi.value;
	return (SUCCESS);
}

int	close_sem_error(t_sem *semaphores)
{
	if (semaphores->forks != SEM_FAILED)
		unlink_close_sem("forks", semaphores->forks);
	if (semaphores->death_occurred != SEM_FAILED)
		unlink_close_sem("death_occurred", semaphores->death_occurred);
	if (semaphores->death_flag != SEM_FAILED)
		unlink_close_sem("death_flag", semaphores->death_flag);
	if (semaphores->print != SEM_FAILED)
		unlink_close_sem("print", semaphores->print);
	if (semaphores->time != SEM_FAILED)
		unlink_close_sem("time", semaphores->time);
	return (ERROR);
}

int	init_semaphores(t_sem *semaphores, int total_philo)
{
	*semaphores = (t_sem){SEM_FAILED,
			SEM_FAILED, SEM_FAILED, SEM_FAILED, SEM_FAILED};
	sem_unlink("forks");
	sem_unlink("death_occurred");
	sem_unlink("death_flag");
	sem_unlink("print");
	sem_unlink("time");
	semaphores->forks = sem_open("forks", O_CREAT, 0644, total_philo);
	if (semaphores->forks == SEM_FAILED)
		return (close_sem_error(semaphores));
	semaphores->death_occurred = sem_open("death_occurred", O_CREAT, 0644, 0);
	if (semaphores->death_occurred == SEM_FAILED)
		return (close_sem_error(semaphores));
	semaphores->death_flag = sem_open("death_flag", O_CREAT, 0644, 1);
	if (semaphores->death_flag == SEM_FAILED)
		return (close_sem_error(semaphores));
	semaphores->print = sem_open("print", O_CREAT, 0644, 1);
	if (semaphores->print == SEM_FAILED)
		return (close_sem_error(semaphores));
	semaphores->time = sem_open("time", O_CREAT, 0644, 1);
	if (semaphores->time == SEM_FAILED)
		return (close_sem_error(semaphores));
	return (SUCCESS);
}

int	init_philo_struct(int argc, char **argv, t_philo *philo)
{
	if (check_and_set_arg(argv[1], &philo->total_philo) == ERROR)
		return (ERROR);
	if (check_and_set_arg(argv[2], &philo->tt_die) == ERROR)
		return (ERROR);
	if (check_and_set_arg(argv[3], &philo->tt_eat) == ERROR)
		return (ERROR);
	if (check_and_set_arg(argv[4], &philo->tt_sleep) == ERROR)
		return (ERROR);
	if (argc == 6)
	{
		if (check_and_set_arg(argv[5], &philo->eat_max) == ERROR)
			return (ERROR);
	}
	else
		philo->eat_max = -1;
	philo->philo_num = 0;
	philo->meals_eaten = 0;
	philo->start_time = (struct timeval){0, 0};
	philo->started_eat = (struct timeval){0, 0};
	philo->started_sleep = (struct timeval){0, 0};
	philo->death_flag = 0;
	return (SUCCESS);
}
