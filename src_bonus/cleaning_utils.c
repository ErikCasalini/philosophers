/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:36:05 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/30 13:36:57 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
#include "philosophers_bonus.h"

void	unlink_close_sem(sem_t *semaphores)
{
	sem_unlink(semaphores);
	sem_close(semaphores);
}

void	close_semaphores(t_sem *semaphores)
{
	unlink_close_sem(&semaphores->forks);
	unlink_close_sem(&semaphores->death);
	unlink_close_sem(&semaphores->sync);
}
