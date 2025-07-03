/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:19:06 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/03 10:09:12 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "philosophers_bonus.h"

static int	wait_children(int children)
{
	while (children)
	{
		waitpid(0, NULL, 0);
		children--;
	}
	return (SUCCESS);
}

static int	create_children(t_philo *philo, t_sem *semaphores)
{
	pid_t	child_pid;

	child_pid = 1;
	while (child_pid != CHILD && ++philo->philo_num <= philo->total_philo)
	{
		child_pid = fork();
		if (child_pid == FAILURE)
		{
			ft_putstr_fd("Fork error: no more philosophers generated"
				" and simulation will be interrupted\n", 2);
			sem_post(semaphores->death_occurred);
			break ;
		}
	}
	if (child_pid == CHILD)
		philo_child_routine(philo, semaphores);
	return (philo->philo_num - 1);
}

int	main(int argc, char *argv[])
{
	t_philo	philo;
	t_sem	semaphores;

	if (argc < 5 || argc > 6)
		exit_bad_argument();
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	if (philo.eat_max == 0)
		return (SUCCESS);
	if (gettimeofday(&philo.start_time, NULL) != SUCCESS)
		exit_print_error("Warning: unable to get time of day\n", 2);
	if (init_semaphores(&semaphores, philo.total_philo) == ERROR)
		exit_print_error("Semaphores creation error\n", 2);
	wait_children(create_children(&philo, &semaphores));
	close_semaphores(&semaphores);
}
