/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:19:06 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/29 17:13:49 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "philosophers_bonus.h"

void	exit_print_error(char *err_msg, int exit_value)
{
	ft_putstr_fd(err_msg, 2);
	exit (exit_value);
}

int	wait_children(int children)
{
	while (children)
	{
		waitpid(0, NULL, 0);
		children--;
	}
	return (SUCCESS);
}

int	init_semaphores(t_sem *semaphores, int total_philo)
{
	if (sem_open(&semaphores->forks, total_philo) < 0)
		return (ERROR);
	if (sem_open(&semaphores->death, 0) < 0)
		return (ERROR);
	if (sem_open(&semaphores->sync, 0) < 0)
		return (ERROR);
	// NEED TO UNLINCK & CO
	return (SUCCESS);
}

void	destroy_semaphores(t_sem *semaphores)
{

}

int	main(int argc, char *argv[])
{
	t_philo	philo;
	t_intf	total_philo;
	pid_t	child_pid;
	int		philo_num;
	t_sem	semaphores;

	if (argc < 5 || argc > 6)
		exit_bad_argument();
	total_philo = ft_atoi_flag(argv[1]);
	if (total_philo.flag == ERROR)
		exit_bad_argument();
	philo_num = 0;
	child_pid = 1;
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	if (init_semaphores(&semaphores) == ERROR)
		exit_print_error("Semaphores creation error\n", 2);
	while (child_pid != CHILD && ++philo_num <= total_philo.value)
	{
		child_pid = fork();
		if (child_pid == FAILURE)
		{
			ft_putstr_fd("Fork error\n", 2);
			philo_num--;
			break ;
		}
	}
	wait_children(philo_num);
	destroy_semaphores(&semaphores);
}
