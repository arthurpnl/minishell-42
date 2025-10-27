/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:53:26 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/27 11:30:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_cmd(t_commande *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (cmd->args[0][0] == '\0')
		return (1);
	return (0);
}

int	can_exec(char *path, t_ctx *ctx)
{
	struct stat	infos;

	if (stat(path, &infos) != 0)
	{
		ft_putstr_fd(SHELL_NAME, 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (ctx->last_status = 127);
	}
	if (!S_ISREG(infos.st_mode))
	{
		ft_putstr_fd(SHELL_NAME, 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		return (ctx->last_status = 126);
	}
	if (!(infos.st_mode & S_IXUSR))
	{
		ft_putstr_fd(SHELL_NAME, 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (ctx->last_status = 126);
	}
	return (0);
}
