/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:46:36 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/25 17:22:17 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS
# define PHILOSOPHERS

#include <sys/time.h>
#include <pthread.h>

# define ERROR 1
# define SUCCESS 0
# define INVALID 1
# define OVERFLOW 2

/* ATOI FLAG */
typedef struct s_intf
{
	int		value;
	char	flag;
}				t_intf;

/* COMMON PHILOSPHER DATA */
typedef struct s_philo
{
	struct timeval	start_time;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				eat_max;
	int				total_philo;
	char			death_flag;
	pthread_mutex_t	*death_mutex;
	pthread_mutex_t	*sync_mutex;
}				t_philo;

/* PHILOSPHER'S FORKS */
typedef struct s_side_forks
{
	int	left;
	int	right;
}				t_side_forks;

/* SPECIFIC PER-THREAD ARGUMENTS */
typedef struct	s_thread_args
{
	t_philo			*philo;
	int				philo_num;
	t_side_forks	side_forks;
	struct timeval	started_eat;
	struct timeval	started_sleep;
	int				meals_eaten;
	char			*forks;
	pthread_mutex_t	*mutexes;
}				t_thread_args;

/* HEAP TO FREE */
typedef struct	s_heap_allocated
{
	char			*forks;
	pthread_mutex_t	*mutexes;
	pthread_t		*thread_lst;
	t_thread_args	*thread_args;
}				t_heap_allocated;

/* ALLOCATE HEAP */
int				allocate_heap(t_heap_allocated *heap, int philo_num);
char			*alloc_forks_array(int fork_num);
pthread_t		*alloc_thread_lst(int thread_num);
t_thread_args	*alloc_thread_args(int thread_num);
pthread_mutex_t	*alloc_mutexes_array(int mutexes_num);

/* DATA INITIALISATION UTILS */
int				init_philo_struct(int argc, char **argv, t_philo *philo, pthread_mutex_t *death_mutex, pthread_mutex_t *sync_mutex);
t_side_forks	set_forks(int current_philo, int philo_num);
int				init_mutexes(pthread_mutex_t *mutexes, int total_philo, pthread_mutex_t *death_mutex, pthread_mutex_t *sync_mutex);

/*UTILS*/
long long		get_ms_diff(struct timeval start, struct timeval current);
long long		curr_timestamp(struct timeval start_time);
t_intf			ft_atoi_flag(char *str);
void			mutex_printf(char *str, t_thread_args *args);

/* FREE UTILS */
void			free_heap(t_heap_allocated *heap);
int				free_heap_exit_err(t_heap_allocated *heap);

/* ARGUMENTS ERRORS UTILS */
void			exit_bad_argument(void);

/* CLEANING UTILS */
int	destroy_mutexes(pthread_mutex_t *mutexes, int total_philo, pthread_mutex_t *death_mutex, pthread_mutex_t *sync_mutex);
int	join_threads(pthread_t *thread_lst, int total_philo);

#endif
