/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:39:44 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/23 15:50:49 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "philosophers.h"
#include <stdlib.h>
#include <string.h>


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

// int	random_time()
// {
// 	srand((unsigned int)time(NULL));
// 	return ((rand() % (8000000 - 1000000 + 1) + 1000000));
// }


// void	*routine(void *args)
// {
// 	t_philo *casted_args;
// 	struct timeval curr_time;
// 	int	ms_diff;

// 	casted_args = (t_philo*)args;
// 	gettimeofday(&curr_time, NULL);
// 	printf("%d : I'm born at %lld ms from program launch\n" , casted_args->philo_num, get_ms_diff(casted_args->start_time, curr_time));
// 	usleep(casted_args->rand_num);
// 	while(1)
// 	{
// 		gettimeofday(&curr_time, NULL);
// 		ms_diff = get_ms_diff(casted_args->start_time, curr_time);
// 		if (ms_diff >= casted_args->tt_die)
// 		{
// 			printf("%d : I died at %d ms after program starting\n", casted_args->philo_num, ms_diff);
// 			return (NULL);
// 		}
// 	}
// 	return (NULL);
// }

// int	main(void)
// {
// 	pthread_t t1;
// 	pthread_t t2;
// 	struct timeval starting_time;
// 	t_philo args1;
// 	t_philo args2;

// 	gettimeofday(&starting_time, NULL);
// 	args1 = (t_philo){1, starting_time, 400, random_time()};
// 	args2 = (t_philo){2, starting_time, 300, random_time()};
// 	// usleep(1000000);
// 	pthread_create(&t1, NULL, routine, &args1);
// 	// usleep(1000000);
// 	pthread_create(&t2, NULL, routine, &args2);
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// }

// int	main(void)
// {
	// 	int i = random_time();
	// 	printf("%d\n", i);
	// 	usleep(i);
	// }

typedef struct s_philo
{
	struct timeval	start_time;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				eat_max;
	char			death_flag;
}				t_philo;

typedef struct s_side_forks
{
	int	left;
	int	right;
}				t_side_forks;

typedef struct	s_thread_args
{
	t_philo			philo;
	int				philo_num;
	t_side_forks	side_forks;
}				t_thread_args;

typedef struct	s_heap_allocated
{
	char			*forks;
	pthread_t		*thread_lst;
	t_thread_args	*thread_args;
}				t_heap_allocated;


int	check_and_set_arg(char *arg, int *philo_param)
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

char *alloc_forks_array(int fork_num)
{
	char *forks;

	forks = malloc(fork_num);
	if (forks == NULL)
		return (NULL);
	memset(forks, 1, fork_num);
	return (forks);
}

pthread_t *alloc_thread_lst(int thread_num)
{
	pthread_t	*thread_lst;

	thread_lst = malloc(sizeof(pthread_t) * thread_num);
	if (thread_lst == NULL)
		return (NULL);
	memset(thread_lst, 0, thread_num);
	return (thread_lst);
}

t_thread_args *alloc_thread_args(int thread_num)
{
	t_thread_args	*thread_args;

	thread_args = malloc(sizeof(t_thread_args) * thread_num);
	if (thread_args == NULL)
		return (NULL);
	memset(thread_args, 0, thread_num);
	return (thread_args);
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

void	*routine(void *args_struct)
{
	t_thread_args *args;

	args = (t_thread_args *)args_struct;
	printf("p_num:%d, my forks are left=%d and right=%d, ttd: %d, tte: %d, tts: %d, eat_max: %d\n", args->philo_num, args->side_forks.left, args->side_forks.right, args->philo.tt_die, args->philo.tt_eat, args->philo.tt_sleep, args->philo.eat_max);
	return (NULL);
}

int	create_threads(t_philo *philo, int philo_num, t_heap_allocated *heap)
{
	int		current_philo;
	t_side_forks side_forks;

	current_philo = 0;
	while (current_philo < philo_num)
	{
		side_forks = set_forks(current_philo, philo_num);
		heap->thread_args[current_philo] = (t_thread_args){*philo, current_philo + 1, side_forks};
		pthread_create(&heap->thread_lst[current_philo], NULL, routine, &heap->thread_args[current_philo]);
		// testt(&heap->thread_args[current_philo]);
		current_philo++;
	}
	return (SUCCESS);
}

void	free_heap(t_heap_allocated *heap)
{
	free(heap->forks);
	free(heap->thread_args);
	free(heap->thread_lst);
}

int	free_heap_exit_err(t_heap_allocated *heap)
{
	free_heap(heap);
	printf("Internal error occurred\n");
	exit(1);
}

void	exit_bad_argument(void)
{
	printf("Bad arguments: provide positive or "
			"null integers in the following order:\n"
			"number_of_philosophers\n"
			"time_to_die\n"
			"time_to_eat\n"
			"time_to_sleep\n"
			"Optional: number_of_times_each_philosopher_must_eat\n");
	exit (2);
}

int	allocate_heap(t_heap_allocated *heap, int philo_num)
{
	heap->forks = alloc_forks_array(philo_num);
	if (heap->forks == NULL)
		return (ERROR);
	heap->thread_lst = alloc_thread_lst(philo_num);
	if (heap->thread_lst == NULL)
		return (ERROR);
	heap->thread_args = alloc_thread_args(philo_num);
	if (heap->thread_args == NULL)
		return (ERROR);
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_philo				philo;
	t_intf				philo_num;
	t_heap_allocated	heap;
	/* HAVE TO FIX ATOI : -> r1234 should not work */
	if (argc < 5 || argc > 6)
		exit_bad_argument();
	heap = (t_heap_allocated){NULL, NULL, NULL};
	philo_num = ft_atoi_flag(argv[1]);
	if (philo_num.flag == ERROR)
		exit_bad_argument();
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	// printf("p_num:%d, ttd: %d, tte: %d, tts: %d, eat_max: %d\n", philo_num.value, philo.tt_die, philo.tt_eat, philo.tt_sleep, philo.eat_max);
	if (allocate_heap(&heap, philo_num.value) == ERROR)
		free_heap_exit_err(&heap);
	create_threads(&philo, philo_num.value, &heap);
	pthread_join(heap.thread_lst[0], NULL);
	pthread_join(heap.thread_lst[1], NULL);
	pthread_join(heap.thread_lst[2], NULL);
	free_heap(&heap);
	return (0);
}
