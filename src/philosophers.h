/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:46:36 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 11:54:20 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <sys/time.h>
# include <pthread.h>

# define ERROR 1
# define SUCCESS 0

/* ATOI FLAG */
typedef enum e_atoif
{
	success,
	negative,
	invalid,
	overflow
}			t_atoif;

typedef struct s_intf
{
	int		value;
	t_atoif	flag;
}				t_intf;

/* COMMON PHILOSPHER DATA */
typedef struct s_philo
{
	struct timeval	start_time;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				tt_think;
	int				eat_max;
	int				total_philo;
	char			death_flag;
	int				return_value;
}				t_philo;

/* PHILOSPHER'S FORKS */
typedef struct s_side_forks
{
	int	left;
	int	right;
}				t_side_forks;

/* MUTEXES */
typedef struct s_mutexes
{
	pthread_mutex_t	*fork_mutexes;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	sync_mutex;
}				t_mutexes;

/* SPECIFIC PER-THREAD ARGUMENTS */
typedef struct s_thread_args
{
	t_philo			*philo;
	int				philo_num;
	t_side_forks	side_forks;
	struct timeval	started_think;
	struct timeval	started_eat;
	struct timeval	started_sleep;
	int				meals_eaten;
	char			*forks;
	t_mutexes		*mutexes;
}				t_thread_args;

/* HEAP TO FREE */
typedef struct s_heap_allocated
{
	char			*forks;
	pthread_mutex_t	*fork_mutexes;
	pthread_t		*thread_lst;
	t_thread_args	*thread_args;
}				t_heap_allocated;

/* ALLOCATE HEAP */
int				allocate_heap(t_heap_allocated *heap, int philo_num);
char			*alloc_forks_array(int fork_num);
pthread_t		*alloc_thread_lst(int thread_num);
t_thread_args	*alloc_thread_args(int thread_num);
pthread_mutex_t	*alloc_fork_mutexes(int mutexes_num);

/* DATA INITIALISATION UTILS */
int				init_philo_struct(int argc, char **argv, t_philo *philo);
t_side_forks	init_forks_indexes(int current_philo, int philo_num);
int				init_mutexes(t_mutexes *mutexes,
					pthread_mutex_t *fork_mutexes, int total_philo);
void			init_thread_args_struct(t_heap_allocated *heap,
					t_philo *philo, int philo_num, int total_philo);

/* ROUTINE */
void			*routine(void *args_struct);
int				everybody_lives(t_thread_args *args);
int				start_thinking(t_thread_args *args);
int				start_eating(t_thread_args *args);
int				start_sleeping(t_thread_args *args);

/* FORK UTILS */
int				try_left_fork(t_thread_args *args, int *holded_forks);
int				try_right_fork(t_thread_args *args, int *holded_forks);
int				drop_forks(t_thread_args *args);

/*UTILS*/
long long		get_ms_diff(struct timeval start, struct timeval current);
long long		curr_timestamp(struct timeval start_time);
t_intf			ft_atoi_flag(char *str);
void			mutex_printf(char *str, t_thread_args *args);
int				is_even(int philo_num);
int				ft_isdigit(int c);

/* FREE UTILS */
void			free_heap(t_heap_allocated *heap);
int				free_heap_exit_err(t_heap_allocated *heap);

/* ARGUMENTS ERRORS UTILS */
void			exit_bad_argument(void);

/* CLEANING UTILS */
int				destroy_mutexes(t_mutexes *mutexes, int total_philo);
int				join_threads(pthread_t *thread_lst, int threads_to_join);

#endif
