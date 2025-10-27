/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:45:25 by arthur            #+#    #+#             */
/*   Updated: 2025/10/27 13:06:52 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_command(t_commande *cmd_list)
{
	int			i;
	t_commande	*current;

	current = cmd_list;
	i = 0;
	while (current != NULL)
	{
		i++;
		current = current->next;
	}
	return (i);
}

int	create_pipes(t_pipeline *pipeline)
{
	int	i;

	i = 0;
	if (pipeline->cmd_count <= 1)
	{
		pipeline->pipes = NULL;
		return (0);
	}
	pipeline->pipes = malloc(sizeof(int *) * (pipeline->cmd_count - 1));
	if (!pipeline->pipes)
		return (-1);
	while (i < pipeline->cmd_count - 1)
	{
		pipeline->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipeline->pipes[i])
			return (free_pipes(pipeline->pipes, i), -1);
		if (pipe(pipeline->pipes[i]) == -1)
			return (free_pipes(pipeline->pipes, i + 1), -1);
		i++;
	}
	return (0);
}

void	init_pipeline(t_pipeline *pipeline, t_commande *cmd_list, char **env)
{
	pipeline->cmd_list = cmd_list;
	pipeline->cmd_count = count_command(cmd_list);
	pipeline->env = env;
	pipeline->last_status = 0;
	pipeline->i = 0;
	pipeline->pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipeline->pids)
		exit(EXIT_FAILURE);
}
