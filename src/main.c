/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/15 16:32:22 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "philosophers.h"


long long	get_ms_diff(struct timeval start, struct timeval current)
{
	int	diff;

	diff = (current.tv_sec - start.tv_sec) * 1000;
	diff += (current.tv_usec - start.tv_usec) / 1000;
	return (diff);
}

int	main(void)
{
	struct timeval starting_time;
	struct timeval curr_time;
	int	i = 0;

	gettimeofday(&starting_time, NULL);
	while(i++ <= 20)
	{
		usleep(1000000);
		gettimeofday(&curr_time, NULL);
		printf("Diff: %lld\n", get_ms_diff(starting_time, curr_time));
	}
}
