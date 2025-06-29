/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init_tools_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:47:58 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/29 16:36:49 by ecasalin         ###   ########.fr       */
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
	philo->start_time = (struct timeval){0, 0};
	return (SUCCESS);
}
