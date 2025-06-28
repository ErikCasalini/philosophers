/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_errors_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:45:41 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 11:34:06 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

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
