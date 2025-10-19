/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:54:31 by arthur            #+#    #+#             */
/*   Updated: 2025/10/19 17:09:43 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_heredoc_redirect(t_redirection *redir)
{
	if (redir->fd == -1)
		return (-1);
	dup2(redir->fd, STDIN_FILENO);
	close(redir->fd);
	return (0);
}

int	dispatch_redirect(t_commande *cmd_list)
{
	t_redirection	*current;
	int				res;

	current = cmd_list->redirection;
	while (current != NULL)
	{
		if (current->type == TOK_REDIR_IN)
			res = handle_input_redirect(current);
		else if (current->type == TOK_REDIR_OUT)
			res = handle_output_redirect(current);
		else if (current->type == TOK_REDIR_APPEND)
			res = handle_append_redirect(current);
		else if (current->type == TOK_HEREDOC)
			res = apply_heredoc_redirect(current);
		if (res != 0)
			return (res);
		current = current->next;
	}
	return (0);
}

int	handle_input_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_append_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
