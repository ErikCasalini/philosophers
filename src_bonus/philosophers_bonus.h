/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:19:55 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/29 17:07:38 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <sys/time.h>
// # include <pthread.h>

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
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				eat_max;
	int				total_philo;
}				t_philo;

/* SEMAPHORES */
typedef struct s_sem
{
	sem_t	forks;
	sem_t	death;
	sem_t	sync;
}				t_sem;

/* UTILS */
t_intf	ft_atoi_flag(char *str);
int		ft_isdigit(int c);
int		ft_putstr_fd(char *s, int fd);

/* ARGS ERRORS UTILS */
void	exit_bad_argument(void);

#endif
