/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/19 13:37:56 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "philosophers.h"
#include <stdlib.h>


long long	get_ms_diff(struct timeval start, struct timeval current)
{
	int	diff;

	diff = (current.tv_sec - start.tv_sec) * 1000;
	diff += (current.tv_usec - start.tv_usec) / 1000;
	return (diff);
}

// int	main(void)
// {
	// struct timeval starting_time;
	// struct timeval curr_time;
	// int	i = 0;
// 
	// gettimeofday(&starting_time, NULL);
	// while(i++ <= 20)
	// {
		// usleep(1000000);
		// gettimeofday(&curr_time, NULL);
		// printf("Diff: %lld\n", get_ms_diff(starting_time, curr_time));
	// }
// }

int	random_time()
{
	srand((unsigned int)time(NULL));
	return ((rand() % (8000000 - 1000000 + 1) + 1000000));
}

typedef struct s_philo
{
	int 			philo_num;
	struct timeval	start_time;
	int				tt_die;
	int				rand_num;
}				t_philo;

void	*routine(void *args)
{
	t_philo *casted_args;
	struct timeval curr_time;
	int	ms_diff;
	
	casted_args = (t_philo*)args;
	gettimeofday(&curr_time, NULL);
	printf("%d : I'm born at %lld ms from program launch\n" , casted_args->philo_num, get_ms_diff(casted_args->start_time, curr_time));
	usleep(casted_args->rand_num);
	while(1)
	{
		gettimeofday(&curr_time, NULL);
		ms_diff = get_ms_diff(casted_args->start_time, curr_time);
		if (ms_diff >= casted_args->tt_die)
		{
			printf("%d : I died at %d ms after program starting\n", casted_args->philo_num, ms_diff);
			return (NULL);
		}
	}
	return (NULL);
}

int	main(void)
{
	pthread_t t1;
	pthread_t t2;
	struct timeval starting_time;
	t_philo args1;
	t_philo args2;
	
	gettimeofday(&starting_time, NULL);
	args1 = (t_philo){1, starting_time, 400, random_time()};
	args2 = (t_philo){2, starting_time, 300, random_time()};
	// usleep(1000000);
	pthread_create(&t1, NULL, routine, &args1);
	// usleep(1000000);
	pthread_create(&t2, NULL, routine, &args2);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}

// int	main(void)
// {
// 	int i = random_time();
// 	printf("%d\n", i);
// 	usleep(i);
// }