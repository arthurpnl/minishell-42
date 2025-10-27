/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 17:04:56 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/27 11:30:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_all(t_ctx *ctx, t_commande *cmd_list)
{
	if (cmd_list)
	{
		free_commande(cmd_list);
		cmd_list = NULL;
	}
	if (ctx)
	{
		if (ctx->env)
		{
			free_envp(ctx->env);
			ctx->env = NULL;
		}
	}
}

void	cleanup_and_exit(t_ctx *ctx, t_commande *cmd_list, int exit_code)
{
	cleanup_all(ctx, cmd_list);
	exit(exit_code);
}
