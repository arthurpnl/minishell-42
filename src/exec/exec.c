/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:54:31 by arthur            #+#    #+#             */
/*   Updated: 2025/10/20 12:45:10 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_dispatch(t_commande *cmd_list, t_shell_ctx *ctx)
{
	int	res;

	if (is_empty_cmd(cmd_list))
		return (ctx->last_status = 0);
	res = process_all_heredocs(cmd_list);
	if (res != 0)
		return (ctx->last_status = res);
	identify_cmd_type(cmd_list);
	if (!cmd_list->next)
	{
		if (cmd_list->type == CMD_BUILTIN)
			ctx->last_status = exec_builtin(cmd_list, ctx);
		else if (cmd_list->type == CMD_SIMPLE)
			ctx->last_status = exec_single_cmd(cmd_list, ctx);
		else if (cmd_list->type == CMD_ABSOLUTE
			|| cmd_list->type == CMD_RELATIVE)
			ctx->last_status = exec_absolute_cmd(cmd_list, ctx);
		else
			ctx->last_status = 1;
	}
	else
		ctx->last_status = exec_pipeline(cmd_list, ctx);
	return (ctx->last_status);
}

int	exec_absolute_cmd(t_commande *cmd_list, t_shell_ctx *ctx)
{
	pid_t	pid;

	if (!cmd_list || !cmd_list->args || !cmd_list->args[0])
		return (1);
	ctx->last_status = can_exec(cmd_list->args[0], ctx);
	if (ctx->last_status != 0)
		return (ctx->last_status);
	pid = fork();
	if (pid == 0)
	{
		if (dispatch_redirect(cmd_list) != 0)
			exit(EXIT_FAILURE);
		execve(cmd_list->args[0], cmd_list->args, ctx->env);
		perror(cmd_list->args[0]);
		exit(126);
	}
	else if (pid > 0)
	{
		waitpid(pid, &ctx->last_status, 0);
		return (ctx->last_status >> 8);
	}
	return (1);
}

int	exec_command_direct(t_commande *cmd_list, t_shell_ctx *ctx)
{
	char	*cmd_path;

	if (cmd_list->type == CMD_SIMPLE)
	{
		cmd_path = create_full_path(cmd_list, ctx->env);
		if (!cmd_path)
			exit(127);
		execve(cmd_path, cmd_list->args, ctx->env);
		free(cmd_path);
	}
	else if (cmd_list->type == CMD_ABSOLUTE || cmd_list->type == CMD_RELATIVE)
		execve(cmd_list->args[0], cmd_list->args, ctx->env);
	exit(127);
}
