/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:54:31 by arthur            #+#    #+#             */
/*   Updated: 2025/10/24 19:34:17 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_dispatch(t_commande *cmd_list, t_shell_ctx *ctx)
{
	int	res;

	res = process_all_heredocs(cmd_list);
	if (res != 0)
		return (ctx->last_status = res);
	identify_cmd_type(cmd_list);
	if (!cmd_list->next)
	{
		if (is_empty_cmd(cmd_list))
    	{
        	//write(2, ": command not found\n", 20);
        	return (ctx->last_status = 0);
    	}
		if (cmd_list->type == CMD_BUILTIN)
			ctx->last_status = exec_builtin(cmd_list, ctx, cmd_list);
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
	int		status;

	status = 0;
	if (!cmd_list || !cmd_list->args || !cmd_list->args[0])
		return (1);
	ctx->last_status = can_exec(cmd_list->args[0], ctx);
	if (ctx->last_status != 0)
		return (ctx->last_status);
	setup_signals(2);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (dispatch_redirect(cmd_list) != 0)
			cleanup_and_exit(ctx, cmd_list, EXIT_FAILURE);
		execve(cmd_list->args[0], cmd_list->args, ctx->env);
		perror(cmd_list->args[0]);
		cleanup_and_exit(ctx, cmd_list, 126);
	}
	waitpid(pid, &status, 0);
	setup_signals(0);
	return (ctx->last_status = analyze_child_status(status));
}

int	exec_command_direct(t_commande *cmd_list, t_commande *full_list, t_shell_ctx *ctx)
{
	char	*cmd_path;
	
	if (is_empty_cmd(cmd_list))
    {
        print_cmd_error(cmd_list->args[0], CMD_NOT_FOUND);
        cleanup_and_exit(ctx, full_list, 127);
    }
	if (cmd_list->type == CMD_SIMPLE)
	{
		cmd_path = create_full_path(cmd_list, ctx->env);
		if (!cmd_path)
		{
			print_cmd_error(cmd_list->args[0], CMD_NOT_FOUND);
			cleanup_and_exit(ctx, full_list, 127);
		}
		execve(cmd_path, cmd_list->args, ctx->env);
		perror(cmd_list->args[0]); 
		free(cmd_path);
	}
	else if (cmd_list->type == CMD_ABSOLUTE || cmd_list->type == CMD_RELATIVE)
	{
		execve(cmd_list->args[0], cmd_list->args, ctx->env);
		perror(cmd_list->args[0]);  
	}
	cleanup_and_exit(ctx, full_list, 127);
	return (127);
}
