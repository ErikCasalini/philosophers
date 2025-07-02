/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:36:05 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/02 16:54:33 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
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
