/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdi <mehdi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:53:09 by mehdi             #+#    #+#             */
/*   Updated: 2025/09/23 11:12:19 by mehdi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	handle_unclosed_quote(char *input, t_ctx *ctx)
{
	ctx->last_status = 2;
	printf("unclosed quote\n");
	free(input);
}

static int	handle_syntax_and_cmd(t_token **head, t_commande **cmds,
	t_ctx *ctx)
{
	if (check_syntax(*head))
	{
		free_tokens(*head);
		ctx->last_status = 2;
		return (1);
	}
	*cmds = tokens_to_command(*head);
	free_tokens(*head);
	if (!*cmds)
	{
		ctx->last_status = 0;
		return (1);
	}
	return (0);
}

static void	process_input(char *input, t_ctx *ctx)
{
	char		*str;
	t_token		*head;
	t_commande	*cmds;

	head = NULL;
	str = clean_space(input);
	free(input);
	if (!str)
	{
		ft_putstr_fd("malloc_error\n", 2);
		ctx->last_status = 2;
		return ;
	}
	if (tokenize_line(&head, str, ctx))
	{
		ft_putstr_fd("minishell: tokenize failed\n", 2);
		free(str);
		ctx->last_status = 2;
		return ;
	}
	free(str);
	if (handle_syntax_and_cmd(&head, &cmds, ctx))
		return ;
	ctx->last_status = command_dispatch(cmds, ctx);
	free_commande(cmds);
}

static int	init_context(t_ctx *ctx, char **envp)
{
	if (!envp)
		return (0);
	ctx->env = ft_cpy_envp(envp);
	if (!ctx->env)
	{
		ft_putstr_fd("malloc_error\n", 2);
		return (0);
	}
	ctx->last_status = 0;
	setup_signals(0);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_ctx	ctx;
	char		*input;

	(void)ac;
	(void)av;
	if (!init_context(&ctx, envp))
		return (1);
	while (1)
	{
		g_signal = 0;  // ← Reset AVANT readline
		input = readline(COLOR_RED "minishell$ " COLOR_RESET);
		if (!input)
			return (exit_shell(&ctx));
		if (handle_signal_interrupt(&input, &ctx))
			continue ;
		if (*input)
			add_history(input);
		if (unclosed_quote(input))
			handle_unclosed_quote(input, &ctx);
		else
			process_input(input, &ctx);
	}
	return (0);
}
