/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:58:42 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/20 13:19:44 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child(t_commande *cmd_list, t_pipeline *pipeline, t_shell_ctx *ctx,
		int i)
{
	if (pipeline->cmd_count > 1)
		handle_pipe_redirect(pipeline->pipes, i, pipeline->cmd_count);
	if (dispatch_redirect(cmd_list) != 0)
	{
		free_pipeline_resources(pipeline);
		cleanup_and_exit(ctx, cmd_list, EXIT_FAILURE);
	}
	if (cmd_list->type == CMD_BUILTIN)
	{
		free_pipeline_resources(pipeline);
		cleanup_and_exit(ctx, cmd_list, exec_builtin(cmd_list, ctx));
	}
	else
		exec_command_direct(cmd_list, ctx);
}

int	close_and_wait(t_pipeline *pipeline, t_shell_ctx *ctx)
{
	int	i;
	int	status;
	int	last_status;

	if (!pipeline || !pipeline->pids)
		return (1);
	last_status = 0;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		if (waitpid(pipeline->pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	ctx->last_status = last_status;
	return (last_status);
}
