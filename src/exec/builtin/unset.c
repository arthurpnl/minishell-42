/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:30:54 by arthur            #+#    #+#             */
/*   Updated: 2025/10/27 11:30:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	remove_env_at(t_ctx *ctx, int idx)
{
	int	j;

	if (!ctx || !ctx->env || !ctx->env[idx])
		return (0);
	free(ctx->env[idx]);
	j = idx;
	while (ctx->env[j + 1])
	{
		ctx->env[j] = ctx->env[j + 1];
		j++;
	}
	ctx->env[j] = NULL;
	return (1);
}

static int	unset_one_arg(t_ctx *ctx, const char *name)
{
	size_t	len;
	int		j;

	len = ft_strlen(name);
	j = 0;
	while (ctx && ctx->env && ctx->env[j])
	{
		if (ft_strncmp(ctx->env[j], name, len) == 0 && ctx->env[j][len] == '=')
			return (remove_env_at(ctx, j));
		j++;
	}
	return (0);
}

int	ft_unset(char **args, t_ctx *ctx)
{
	int	i;

	i = 1;
	if (!args || !args[1])
		return (0);
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			unset_one_arg(ctx, args[i]);
		i++;
	}
	return (0);
}
