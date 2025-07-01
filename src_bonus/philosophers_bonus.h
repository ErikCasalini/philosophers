/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:19:55 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/01 17:33:20 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <sys/time.h>
# include <semaphore.h>

# define FAILURE -1
# define ERROR 1
# define SUCCESS 0
# define CHILD 0

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
	struct timeval	started_eat;
	struct timeval	started_sleep;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				meals_eaten;
	int				eat_max;
	int				total_philo;
	int				philo_num;
	int				death_flag;
}				t_philo;

/* SEMAPHORES */
typedef struct s_sem
{
	sem_t	*forks;
	sem_t	*death;
	sem_t	*sync;
	sem_t	*var;
	sem_t	*print;
	sem_t	*time;
}				t_sem;

/* THREADS_ARGS */
typedef struct s_threads_args
{
	t_philo	*philo;
	t_sem	*semaphores;
}				t_threads_args;

/* INIT UTILS */
int		init_philo_struct(int argc, char **argv, t_philo *philo);
int		init_semaphores(t_sem *semaphores, int total_philo);

/* UTILS */
t_intf		ft_atoi_flag(char *str);
int			ft_isdigit(int c);
int			ft_putstr_fd(char *s, int fd);
long long	get_ms_diff(struct timeval start, struct timeval current);
long long	curr_timestamp(struct timeval start_time, sem_t *time);
int			semlock_printf(char *str, t_philo *philo, t_sem *semaphores);
void		semlock_gettimeofday(struct timeval *time_buffer, sem_t *time);
int			is_death_flag(t_philo *philo, t_sem *semaphores);
int			is_even(int philo_num);

/* CLEANING UTILS */
void	unlink_close_sem(char *sem_name, sem_t *semaphores);
void	close_semaphores(t_sem *semaphores);

/* PRINT ERRORS UTILS */
void	exit_bad_argument(void);
int		print_err_return_err(char *err_msg);

#endif
