/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:35:35 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/24 11:04:10 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_ms_diff(struct timeval start, struct timeval current)
{
	int	diff;

	diff = (current.tv_sec - start.tv_sec) * 1000;
	diff += (current.tv_usec - start.tv_usec) / 1000;
	return (diff);
}

long long	curr_timestamp(struct timeval start_time)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	return (get_ms_diff(start_time, current_time));
}
