/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:47:58 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/24 08:49:51 by ecasalin         ###   ########.fr       */
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

int	init_philo_struct(int argc, char **argv, t_philo *philo)
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
	gettimeofday(&philo->start_time, NULL);
	philo->death_flag = 0;
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
