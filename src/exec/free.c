/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:59:16 by arthur            #+#    #+#             */
/*   Updated: 2025/10/13 16:13:09 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	if (!pipes || count <= 1)
		return ;
	while (i < count - 1)
	{
		if (pipes[i])
			free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	free_pipeline_resources(t_pipeline *pipeline)
{
	if (!pipeline)
		return ;
	if (pipeline->pipes)
		free_pipes(pipeline->pipes, pipeline->cmd_count);  // ← Utilise free_pipes()
	if (pipeline->pids)
		free(pipeline->pids);
	free(pipeline);
}

void	close_all_pipes(int **pipes, int count)
{
	int i;

	i = 0;
	if (!pipes)
		return ;
	while (i < count - 1)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
		i++;
	}
}