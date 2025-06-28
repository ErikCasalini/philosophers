/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:36:35 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/28 09:17:05 by ecasalin         ###   ########.fr       */
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

int	free_heap_exit_err(t_heap_allocated *heap)
{
	free_heap(heap);
	printf("Internal error occurred\n");
	exit(1);
}
