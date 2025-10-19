/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 17:07:14 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/19 17:11:28 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe_redirect(int **pipes, int i, int cmd_count)
{
	int	j;

	if (i == 0)
		dup2(pipes[0][1], STDOUT_FILENO);
	else if (i > 0 && i < cmd_count - 1)
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	else if (i == cmd_count - 1 && cmd_count > 1)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
	return (0);
}
