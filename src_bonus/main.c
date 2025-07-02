/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:19:06 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/02 14:03:35 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "philosophers_bonus.h"

void	exit_print_error(char *err_msg, int exit_value)
{
	ft_putstr_fd(err_msg, 2);
	exit (exit_value);
}

int	wait_children(int children)
{
	while (children)
	{
		waitpid(0, NULL, 0);
		children--;
	}
	return (SUCCESS);
}

long long	hunger_counter(struct timeval last_meal, t_sem *semaphores)
{
	struct timeval	current_time;
	long long		ms_diff;
	
	semlock_gettimeofday(&current_time, semaphores->time);
	return (get_ms_diff(last_meal, current_time));
}

void *death_tracker_routine(void *arg_lst)
{
	t_threads_args	*args;

	args = (t_threads_args *)arg_lst;
	while (1)
	{
		// sem_wait(args->semaphores->death_flag);
		if (hunger_counter(args->philo->started_eat, args->semaphores) >= args->philo->tt_die)
		{
			sem_wait(args->semaphores->print);
			// sem_wait(args->semaphores->death_flag);
			// if (!args->philo->death_flag) // Si jamais printef a precedamment echoué le flag est deja la, et on ne doit pas ecrire
			// sem_post(args->semaphores->death_flag);
			// semlock_printf("%lld %d died\n", args->philo, args->semaphores);
			if (!is_death_flag(args->philo, args->semaphores))
				printf("%lld %d died\n", curr_timestamp(args->philo->start_time, args->semaphores->time), args->philo->philo_num);
			sem_post(args->semaphores->death_occurred);
			// sem_wait(args->semaphores->death_flag);
			// args->philo->death_flag = 1;
			// sem_post(args->semaphores->death_flag);
			if (args->philo->total_philo >= 20)
				usleep(20 * args->philo->total_philo);
			else
				usleep(400);
			sem_post(args->semaphores->print);
			return (NULL);
		}
		// sem_post(args->semaphores->death_flag);
		// usleep(250);
	}
	return ((void *)SUCCESS);
}

void *death_flag_setter_routine(void *arg_lst)
{
	t_threads_args	*args;

	args = (t_threads_args *)arg_lst;
	sem_wait(args->semaphores->death_occurred);
	sem_post(args->semaphores->death_occurred);
	sem_wait(args->semaphores->death_flag);
	args->philo->death_flag = 1;
	sem_post(args->semaphores->death_flag);
	return (NULL);
}

int create_threads(t_threads_args *args, pthread_t *death_tracker, pthread_t *death_flag_setter)
{
	if (pthread_create(death_tracker, NULL, death_tracker_routine, args) == ERROR)
	{
		sem_post(args->semaphores->death_occurred);
		return(print_err_return_err("Thread creation error\n"));
	}
	if (pthread_create(death_flag_setter, NULL, death_flag_setter_routine, args) == ERROR)
	{
		sem_post(args->semaphores->death_occurred);
		usleep(3000);
		return(print_err_return_err("Thread creation error\n"));
	}
	return (SUCCESS);
}

void subprocess_close_sem_exit(t_sem *semaphores, pthread_t thread_1, pthread_t thread_2)
{
	void *return_value;

	pthread_join(thread_1, &return_value);
	pthread_join(thread_2, NULL);
	sem_close(semaphores->death_occurred);
	sem_close(semaphores->forks);
	sem_close(semaphores->death_flag);
	sem_close(semaphores->print);
	sem_close(semaphores->time);
	exit(0);
}

int	start_thinking(t_philo *philo, t_sem *semaphores)
{
	usleep(250);
	if (is_death_flag(philo, semaphores))
		return (ERROR);
	semlock_printf("%lld %d is thinking\n", philo, semaphores);
	sem_wait(semaphores->forks);
	semlock_printf("%lld %d has taken a fork\n", philo, semaphores);
	if (is_death_flag(philo, semaphores) || philo->total_philo == 1)
	{
		sem_post(semaphores->forks);
		return (ERROR);
	}
	sem_wait(semaphores->forks);
	semlock_printf("%lld %d has taken a fork\n", philo, semaphores);
	if (is_death_flag(philo, semaphores) || philo->total_philo == 1)
	{
		sem_post(semaphores->forks);
		sem_post(semaphores->forks);
		return (ERROR);
	}
	return (SUCCESS);
}

int	start_eating(t_philo *philo, t_sem *semaphores)
{
	semlock_gettimeofday(&philo->started_eat, semaphores->time);
	semlock_printf("%lld %d is eating\n", philo, semaphores);
	while (1)
	{
		if (is_death_flag(philo, semaphores))
		{
			sem_post(semaphores->forks);
			sem_post(semaphores->forks);
			return (ERROR);
		}
		if (curr_timestamp(philo->started_eat, semaphores->time) >= philo->tt_eat)
			break ;
		usleep(250);
	}
	sem_post(semaphores->forks);
	sem_post(semaphores->forks);
	philo->meals_eaten++;
	return (SUCCESS);
}

int	start_sleeping(t_philo *philo, t_sem *semaphores)
{
	semlock_gettimeofday(&philo->started_sleep, semaphores->time);
	semlock_printf("%lld %d is sleeping\n", philo, semaphores);
	while (1)
	{
		if (is_death_flag(philo, semaphores))
			return (ERROR);
		if (curr_timestamp(philo->started_sleep, semaphores->time) >= philo->tt_sleep)
			break ;
		usleep(250);
	}
	return (SUCCESS);
}

void	philo_routine_and_exit(t_philo *philo, t_sem *semaphores)
{
	pthread_t		death_tracker;
	pthread_t		death_flag_setter;
	t_threads_args	args;

	args.philo = philo;
	args.semaphores = semaphores;
	philo->started_eat = philo->start_time;
	if (create_threads(&args, &death_tracker, &death_flag_setter) == ERROR)
		subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter); // A FIX (PAS DE PTHREAD JOIN ICI)
	if (is_even(philo->philo_num))
		if (start_sleeping(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter);
	while (1)
	{
		if (philo->eat_max != -1
			&& philo->meals_eaten == philo->eat_max)
		{
			sem_post(semaphores->death_occurred);
			subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter);
		}
		if (start_thinking(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter);
		if (start_eating(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter);
		if (philo->eat_max != -1
			&& philo->meals_eaten == philo->eat_max) // ADD THIS TO MAIN PROJECT
		{
			sem_post(semaphores->death_occurred);
			subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter);
		}
		if (start_sleeping(philo, semaphores) == ERROR)
			subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter);
	}
	subprocess_close_sem_exit(semaphores, death_tracker, death_flag_setter);
}

int	main(int argc, char *argv[])
{
	t_philo	philo;
	pid_t	child_pid;
	t_sem	semaphores;

	if (argc < 5 || argc > 6)
		exit_bad_argument();
	if (init_philo_struct(argc, argv, &philo) == ERROR)
		exit_bad_argument();
	if (init_semaphores(&semaphores, philo.total_philo) == ERROR)
		exit_print_error("Semaphores creation error\n", 2);
	child_pid = 1;
	gettimeofday(&philo.start_time, NULL);
	while (child_pid != CHILD && ++philo.philo_num <= philo.total_philo)
	{
		child_pid = fork();
		if (child_pid == FAILURE)
		{
			ft_putstr_fd("Fork error\n", 2);
			philo.philo_num--;
			break ;
		}
	}
	if (child_pid == CHILD)
		philo_routine_and_exit(&philo, &semaphores);
	wait_children(philo.total_philo);
	close_semaphores(&semaphores);
}
