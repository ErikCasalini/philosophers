/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_errors_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:45:41 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/30 11:49:19 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philosophers_bonus.h"

void	exit_bad_argument(void)
{
	ft_putstr_fd("Bad arguments: provide positive or "
		"null integers in the following order:\n"
		"number_of_philosophers\n"
		"time_to_die\n"
		"time_to_eat\n"
		"time_to_sleep\n"
		"Optional: number_of_times_each_philosopher_must_eat\n", 2);
	exit (2);
}
