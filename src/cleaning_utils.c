/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 07:32:03 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/25 07:54:53 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>

int	destroy_mutexes(pthread_mutex_t *mutexes, int total_philo, pthread_mutex_t *death_mutex)
{
	int	i;

	i = 0;
	while (i < total_philo)
	{
		pthread_mutex_destroy(&mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(death_mutex);
	return (SUCCESS);
}

int	join_threads(pthread_t *thread_lst, int total_philo)
{
	int	i;

	i = 0;
	while (i < total_philo)
	{
		pthread_join(thread_lst[i], NULL);
		i++;
	}
	return (SUCCESS);
}
