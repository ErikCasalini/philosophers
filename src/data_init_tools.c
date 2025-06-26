/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:47:58 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/26 14:55:34 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
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

int	init_tt_think(int tt_eat, int tt_sleep)
{
	if (tt_eat < tt_sleep)
		return (0);
	else
		return (tt_eat - tt_sleep);
	// return ((tt_eat * 2) - tt_sleep);
}

int	init_philo_struct(int argc, char **argv, t_philo *philo, pthread_mutex_t *death_mutex, pthread_mutex_t *sync_mutex)
{
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
	philo->death_flag = 0;
	philo->death_mutex = death_mutex;
	philo->sync_mutex = sync_mutex;
	philo->tt_think = init_tt_think(philo->tt_eat, philo->tt_sleep);
	return (SUCCESS);
}

int init_mutexes(pthread_mutex_t *mutexes, int total_philo, pthread_mutex_t *death_mutex, pthread_mutex_t *sync_mutex)
{
	int	i;

	i = 0;
	while (i < total_philo)
	{
		pthread_mutex_init(&mutexes[i], NULL);
		i++;
	}
	pthread_mutex_init(death_mutex, NULL);
	pthread_mutex_init(sync_mutex, NULL);
	return (SUCCESS);
}

t_side_forks set_forks(int current_philo, int philo_num)
{
	t_side_forks	side_forks;

	if (current_philo == 0)
	{
		side_forks.left = philo_num - 1;
		side_forks.right = current_philo;
	}
	else
	{
		side_forks.left = current_philo - 1;
		side_forks.right = current_philo;
	}
	return (side_forks);
}
