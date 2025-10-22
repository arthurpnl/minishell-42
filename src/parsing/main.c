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

void	sig_handler(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_handler_heredoc(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
}

void	setup_signals(int mode)
{
	struct sigaction	sa;
	
	sigemptyset(&sa.sa_mask);
	
	if (mode == 0)  // Mode PROMPT
	{
		sa.sa_handler = sig_handler;
		sa.sa_flags = SA_RESTART;  // ← AVEC restart
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 1)  // Mode HEREDOC
	{
		sa.sa_handler = sig_handler_heredoc;
		sa.sa_flags = 0;  // ← SANS restart → readline s'interrompt !
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 2)  // Mode EXEC (ignore)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}

static int	exit_shell(t_shell_ctx *ctx)
{
	printf("exit\n");
	free_envp(ctx->env);
	clear_history();
	exit(ctx->last_status);
}

static void	handle_unclosed_quote(char *input, t_shell_ctx *ctx)
{
	ctx->last_status = 2;
	printf("unclosed quote\n");
	free(input);
}

static void	process_input(char *input, t_shell_ctx *ctx)
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
	if (check_syntax(head))
	{
		free(str);
		free_tokens(head);
		ctx->last_status = 2;
		return ;
	}
	cmds = tokens_to_command(head);
	if (!cmds)
	{
		free(str);
		free_tokens(head);
		ctx->last_status = 0;
		return ;
	}
	free(str);
	free_tokens(head);
	ctx->last_status = command_dispatch(cmds, ctx);
	free_commande(cmds);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_shell_ctx	ctx;

	(void)ac;
	(void)av;
	if (!envp)
		return (0);
	ctx.env = ft_cpy_envp(envp);
	if (!ctx.env)
	{
		ft_putstr_fd("malloc_error\n", 2);
		return (1);
	}
	ctx.last_status = 0;
	setup_signals(0);
	while (1)
	{
		input = readline(COLOR_RED "minishell$ " COLOR_RESET);
		if (g_signal == SIGINT)
		{
			ctx.last_status = 130;
			g_signal = 0;  // Reset
			if (input)
			{
				free(input);
				input = NULL;
			}
			continue;  // Retour au prompt
		}
		if (!input)
			return (exit_shell(&ctx)); // libère ctx.env et sort proprement
		if (*input)
			add_history(input);
		if (unclosed_quote(input))
			handle_unclosed_quote(input, &ctx);
		else
			process_input(input, &ctx);
	}
	return (0);
}
