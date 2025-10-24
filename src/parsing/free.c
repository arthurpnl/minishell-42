/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:15:51 by mehdi             #+#    #+#             */
/*   Updated: 2025/10/24 17:24:28 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	free_redirection(t_redirection *redir)
{
	t_redirection	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->fd != -1)
		{
			close(tmp->fd);
			tmp->fd = -1;
		}
		if (tmp->file)
			free(tmp->file);
		free(tmp);
	}
}

void	free_commande(t_commande *cmd)
{
	t_commande	*tmp;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		free_args(tmp->args);
		free_redirection(tmp->redirection);
		if (tmp->path)
			free_split(tmp->path);
		free(tmp);
	}
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

int	exit_shell(t_shell_ctx *ctx)
{
	printf("exit\n");
	free_envp(ctx->env);
	clear_history();
	exit(ctx->last_status);
}
