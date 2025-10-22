/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:47:50 by arthur            #+#    #+#             */
/*   Updated: 2025/10/22 17:57:07 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_it_delimiter(char *line, char *delimiter)
{
	size_t	delimiter_len;

	if (!line)
		return (1);
	delimiter_len = ft_strlen(delimiter);
	if (ft_strncmp(line, delimiter, delimiter_len) == 0)
	{
		if (line[delimiter_len] == '\n' || line[delimiter_len] == '\0')
			return (1);
	}
	return (0);
}

static int	write_heredoc_lines(int fd, t_redirection *redir)
{
	char	*line;

	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			if (line)
				free(line);
			return (-1);
		}
		if (is_it_delimiter(line, redir->file))
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	return (0);
}

int	handle_heredoc_redirect(t_redirection *redir)
{
	int		fd_tmp;
	int		fd_read;
	char	*file_name;
	int		res;
	
	file_name = ft_strdup("/tmp/.minishell_heredoc_tmp");
	if (!file_name)
		return (-1);
	fd_tmp = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd_tmp < 0)
		return (free(file_name), -1);
	setup_signals(1);
	res = write_heredoc_lines(fd_tmp, redir);
	close(fd_tmp);
	setup_signals(0);
	if (res == -1)
	{
		unlink(file_name);
		free(file_name);
		return (-1);
	}
	fd_read = open(file_name, O_RDONLY);
	unlink(file_name);
	free(file_name);
	if (redir->fd != -1)
		close(redir->fd);
	redir->fd = fd_read;
	return (0);
}

int	process_all_heredocs(t_commande *cmd_list)
{
	t_commande		*current_cmd;
	t_redirection	*redir;
	int				res;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		redir = current_cmd->redirection;
		while (redir)
		{
			if (redir->type == TOK_HEREDOC)
			{
				res = handle_heredoc_redirect(redir);
				if (res == -1)
					return (130);
			}
			redir = redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}
