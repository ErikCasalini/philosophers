/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:36:05 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/03 10:15:51 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
#include <stdlib.h>
#include "philosophers_bonus.h"

void	unlink_close_sem(char *sem_name, sem_t *semaphore)
{
	sem_unlink(sem_name);
	sem_close(semaphore);
}

void	close_semaphores(t_sem *semaphores)
{
	unlink_close_sem("forks", semaphores->forks);
	unlink_close_sem("death_occurred", semaphores->death_occurred);
	unlink_close_sem("death_flag", semaphores->death_flag);
	unlink_close_sem("print", semaphores->print);
	unlink_close_sem("time", semaphores->time);
	unlink_close_sem("meals_eaten", semaphores->meals_eaten);
}

void	subprocess_close_sem_exit(t_sem *semaphores,
		pthread_t thread_1, pthread_t thread_2)
{
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	sem_close(semaphores->death_occurred);
	sem_close(semaphores->forks);
	sem_close(semaphores->death_flag);
	sem_close(semaphores->print);
	sem_close(semaphores->time);
	sem_close(semaphores->meals_eaten);
	exit(SUCCESS);
}
