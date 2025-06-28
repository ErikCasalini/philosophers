/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 07:32:03 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 10:28:46 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>

int	destroy_mutexes(t_mutexes *mutexes, int total_philo)
{
	int	i;

	i = 0;
	while (i < total_philo)
	{
		pthread_mutex_destroy(&mutexes->fork_mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(&mutexes->death_mutex);
	pthread_mutex_destroy(&mutexes->sync_mutex);
	return (SUCCESS);
}

int	join_threads(pthread_t *thread_lst, int threads_to_join)
{
	int	i;

	i = 0;
	while (i < threads_to_join)
	{
		pthread_join(thread_lst[i], NULL);
		i++;
	}
	return (SUCCESS);
}
