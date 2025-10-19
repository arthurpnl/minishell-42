/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:58:17 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/19 16:56:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin_cmd(t_commande *cmd_list, t_shell_ctx *ctx, t_token *tokens)
{
	if (!cmd_list || !cmd_list->args || !cmd_list->args[0])
		return (1);
	if (ft_strcmp(cmd_list->args[0], "echo") == 0)
		ctx->last_status = ft_echo(cmd_list->args);
	else if (ft_strcmp(cmd_list->args[0], "cd") == 0)
		ctx->last_status = ft_cd(cmd_list->args, ctx);
	else if (ft_strcmp(cmd_list->args[0], "pwd") == 0)
		ctx->last_status = ft_pwd();
	else if (ft_strcmp(cmd_list->args[0], "export") == 0)
		ctx->last_status = ft_export(cmd_list->args, ctx);
	else if (ft_strcmp(cmd_list->args[0], "unset") == 0)
		ctx->last_status = ft_unset(cmd_list->args, ctx);
	else if (ft_strcmp(cmd_list->args[0], "env") == 0)
		ctx->last_status = ft_env(ctx->env);
	else if (ft_strcmp(cmd_list->args[0], "exit") == 0)
		ctx->last_status = ft_exit(cmd_list->args, ctx, cmd_list, tokens);
	else
		ctx->last_status = 1;
	return (ctx->last_status);
}

int	exec_builtin(t_commande *cmd_list, t_shell_ctx *ctx, t_token *tokens)
{
	int	pid;

	if (!cmd_list || !cmd_list->args || !cmd_list->args[0])
		return (1);
	if (!cmd_list->redirection)
		return (exec_builtin_cmd(cmd_list, ctx, tokens));
	pid = fork();
	if (pid == 0)
	{
		if (dispatch_redirect(cmd_list) != 0)
			exit(EXIT_FAILURE);
		exit(exec_builtin_cmd(cmd_list, ctx, tokens));
	}
	else if (pid > 0)
	{
		waitpid(pid, &ctx->last_status, 0);
		return (ctx->last_status >> 8);
	}
	return (ctx->last_status = 1);
}
