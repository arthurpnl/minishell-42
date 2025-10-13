/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:47:50 by arthur            #+#    #+#             */
/*   Updated: 2025/10/13 14:15:50 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_it_delimiter(char *line, char *delimiter)
{
    size_t delimiter_len;

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

int handle_heredoc_redirect(t_redirection *redir)
{
    int     fd_tmp;
    int     fd_read;
    char    *line;
    char    *file_name;

    file_name = ft_strdup("/tmp/.minishell_heredoc_tmp");
    if (!file_name)
        return (-1);
    fd_tmp = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd_tmp < 0)
        return (free(file_name), -1);
    while (1)
    {
        line = readline("heredoc>");
        if (!line || is_it_delimiter(line, redir->file))  // CHANGE ICI
        {
            if (line)
                free(line);
            break ;
        }
        write(fd_tmp, line, ft_strlen(line));
        write(fd_tmp, "\n", 1);
        free(line);
    }
    close(fd_tmp);
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
				if (res != 0)
					return (res);
			}
			redir = redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}