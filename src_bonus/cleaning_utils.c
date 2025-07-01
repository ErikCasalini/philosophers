/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:36:05 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/01 14:58:54 by ecasalin         ###   ########.fr       */
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
	unlink_close_sem("death", semaphores->death);
	unlink_close_sem("sync", semaphores->sync);
	unlink_close_sem("var", semaphores->var);
	unlink_close_sem("print", semaphores->print);
	unlink_close_sem("time", semaphores->time);
}
