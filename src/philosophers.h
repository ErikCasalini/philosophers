/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:46:36 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/23 10:35:29 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS
# define PHILOSOPHERS

# define ERROR 1
# define SUCCESS 0
# define INVALID 1
# define OVERFLOW 2

typedef struct s_intf
{
	int		value;
	char	flag;
}				t_intf;

/*UTILS*/
t_intf	ft_atoi_flag(char *str);


#endif
