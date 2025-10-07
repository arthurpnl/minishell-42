/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdi <mehdi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:03:02 by mehdi             #+#    #+#             */
/*   Updated: 2025/09/24 19:08:10 by mehdi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(t_token_type type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_REDIR_APPEND || type == TOK_HEREDOC);
}

int	is_word(t_token *token)
{
	return (token && token->type == TOK_WORD && token->word != NULL);
}

int	print_pipe_error(void)
{
	ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
	return (2);
}

int	print_redir_error(t_token *next)
{
	if (!next)
	{
		ft_putstr_fd("syntax error: unexpected end after redirection\n", 2);
		return (2);
	}
	ft_putstr_fd("syntax error near unexpected token ", 2);
	if (next->type == TOK_PIPE)
		ft_putstr_fd("`|'\n", 2);
	else
		ft_putstr_fd("`<' or `>'\n", 2);
	return (2);
}

int	print_heredoc_error(void)
{
	ft_putstr_fd("syntax error: heredoc requires a delimiter\n", 2);
	return (2);
}
