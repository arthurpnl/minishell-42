/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:55:32 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/24 17:12:18 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	analyze_child_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			return (130);
		if (sig == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", 2);
			return (131);
		}
		return (128 + sig);
	}
	return (1);
}

void sig_handler(int sig)
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
	if (mode == 0)
	{
		sa.sa_handler = sig_handler;
		sa.sa_flags = SA_RESTART;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 1)
	{
		sa.sa_handler = sig_handler_heredoc;
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 2)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}

int handle_signal_interrupt(char **input, t_shell_ctx *ctx)
{
    if (g_signal == SIGINT)
    {
        ctx->last_status = 130;
        g_signal = 0;
        
        if (!*input || !**input)
        {
            if (*input)
                free(*input);
            return (1);
        }
        return (0);
    }
    return (0);
}
