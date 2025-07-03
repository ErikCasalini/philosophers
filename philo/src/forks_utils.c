/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:02:06 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 11:02:34 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	try_left_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	if (args->forks[args->side_forks.left])
	{
		args->forks[args->side_forks.left] = 0;
		*holded_forks += 1;
		mutex_printf("%lld %d has taken a fork\n", args);
	}
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	return (SUCCESS);
}

int	try_right_fork(t_thread_args *args, int *holded_forks)
{
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.right]);
	if (args->forks[args->side_forks.right])
	{
		args->forks[args->side_forks.right] = 0;
		*holded_forks += 1;
		mutex_printf("%lld %d has taken a fork\n", args);
	}
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.right]);
	return (SUCCESS);
}

int	drop_forks(t_thread_args *args)
{
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	args->forks[args->side_forks.left] = 1;
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.left]);
	pthread_mutex_lock(&args->mutexes->fork_mutexes[args->side_forks.right]);
	args->forks[args->side_forks.right] = 1;
	pthread_mutex_unlock(&args->mutexes->fork_mutexes[args->side_forks.right]);
	return (SUCCESS);
}
