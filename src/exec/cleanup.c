/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 17:04:56 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/19 17:05:04 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_all(t_shell_ctx *ctx, t_commande *cmd_list, t_token *tokens)
{
	if (cmd_list)
	{
		free_commande(cmd_list);
		cmd_list = NULL;
	}
	if (tokens)
	{
		free_tokens(tokens);
		tokens = NULL;
	}
	if (ctx && ctx->env)
	{
		free_envp(ctx->env);
		ctx->env = NULL;
	}
}

void	cleanup_and_exit(t_shell_ctx *ctx, t_commande *cmd_list,
		t_token *tokens, int exit_code)
{
	cleanup_all(ctx, cmd_list, tokens);
	exit(exit_code);
}
