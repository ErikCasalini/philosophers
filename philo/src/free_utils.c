/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:36:35 by ecasalin          #+#    #+#             */
/*   Updated: 2025/07/03 14:42:35 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "stdlib.h"
#include "stdio.h"

void	free_heap(t_heap_allocated *heap)
{
	free(heap->forks);
	free(heap->thread_args);
	free(heap->thread_lst);
	free(heap->fork_mutexes);
}

int	free_heap_exit_err(char *err_msg, t_heap_allocated *heap)
{
	free_heap(heap);
	ft_putstr_fd(err_msg, 2);
	exit(1);
}
