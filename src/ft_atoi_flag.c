/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_flag.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 10:00:22 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/23 13:57:04 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

static int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

static int	is_sign(char c, int *sign, t_intf *n)
{
	if (c == '+')
		return (1);
	if (c == '-')
	{
		n->flag = INVALID;
		return (1);
	}
	return (0);
}

static int	set_flag(t_intf *n, int flag)
{
	n->flag = flag;
	return (ERROR);
}

static int	is_overflow(t_intf *n, char digit)
{
	int	tmp;

	if (n->value == 0)
		return (0);
	tmp = n->value;
	if (tmp * 10 / n->value != 10)
		return (set_flag(n, OVERFLOW));
	tmp *= 10;
	if (digit > 0)
		if (tmp + digit <= 0)
			return (set_flag(n, OVERFLOW));
	if (digit < 0)
		if (tmp + digit >= 0)
			return (set_flag(n, OVERFLOW));
	return (0);
}

t_intf	ft_atoi_flag(char *str)
{
	int		sign;
	int		i;
	t_intf	n;

	i = 0;
	n = (t_intf){0, 0};
	sign = 1;
	while (ft_isspace(str[i]))
		i++;
	if (is_sign(str[i], &sign, &n))
		i++;
	if (n.flag == INVALID)
		return (n);
	while (ft_isdigit(str[i]))
	{
		n.flag = 0;
		if (is_overflow(&n, (str[i] - 48)))
			return (n);
		n.value = (n.value * 10) + ((str[i] - 48));
		i++;
		if (str[i] != '\0' && !ft_isspace(str[i]))
			n.flag = INVALID;
	}
	return (n);
}
