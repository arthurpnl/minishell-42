/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 00:00:00 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/27 11:30:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_single_child(t_commande *cmd_list, t_ctx *ctx)
{
	char	*cmd_path;

	if (dispatch_redirect(cmd_list) != 0)
		cleanup_and_exit(ctx, cmd_list, EXIT_FAILURE);
	cmd_path = create_full_path(cmd_list, ctx->env);
	if (!cmd_path)
	{
		print_cmd_error(cmd_list->args[0], CMD_NOT_FOUND);
		cleanup_and_exit(ctx, cmd_list, 127);
	}
	execve(cmd_path, cmd_list->args, ctx->env);
	perror(cmd_list->args[0]);
	free(cmd_path);
	cleanup_and_exit(ctx, cmd_list, EXIT_FAILURE);
}

int	exec_single_cmd(t_commande *cmd_list, t_ctx *ctx)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!cmd_list || !cmd_list->args)
		return (ctx->last_status = 1);
	if (!cmd_list->args[0] || cmd_list->args[0][0] == '\0')
		return (ctx->last_status = 0);
	setup_signals(2);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_single_child(cmd_list, ctx);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		setup_signals(0);
		ctx->last_status = analyze_child_status(status);
	}
	return (ctx->last_status);
}

static int	fork_pipeline_child(t_commande *curr, t_pipeline *pipeline,
				t_ctx *ctx, int i, t_commande *cmd_list)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free_pipeline_resources(pipeline);
		return (-1);
	}
	if (pid == 0)
	{
		exec_child(curr, pipeline, ctx, i, cmd_list);
		free_pipeline_resources(pipeline);
		cleanup_and_exit(ctx, cmd_list, EXIT_FAILURE);
	}
	return (pid);
}

int	exec_pipeline(t_commande *cmd_list, t_ctx *ctx)
{
	t_pipeline	*pipeline;
	t_commande	*curr;
	int			i;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (1);
	init_pipeline(pipeline, cmd_list, ctx->env);
	if (create_pipes(pipeline) != 0)
		return (1);
	setup_signals(2);
	i = 0;
	curr = cmd_list;
	while (curr)
	{
		pipeline->pids[i] = fork_pipeline_child(curr, pipeline, ctx, i, cmd_list);
		curr = curr->next;
		i++;
	}
	close_all_pipes(pipeline->pipes, pipeline->cmd_count);
	ctx->last_status = close_and_wait(pipeline, ctx);
	setup_signals(0);
	free_pipeline_resources(pipeline);
	return (ctx->last_status);
}
