/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_command_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdi <mehdi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:05:24 by mehdi             #+#    #+#             */
/*   Updated: 2025/10/03 19:32:01 by mehdi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	total_len(t_token_word *word)
{
	size_t	len;

	len = 0;
	while (word)
	{
		len += ft_strlen(word->word);
		word = word->next;
	}
	return (len);
}

char	*join_token_words(t_token_word *word)
{
	size_t			len;
	char			*res;
	t_token_word	*tmp;

	len = total_len(word);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	tmp = word;
	while (tmp)
	{
		ft_strcat(res, tmp->word);
		tmp = tmp->next;
	}
	return (res);
}

void	append_redir(t_redirection **list, t_redirection *new)
{
	t_redirection	*last;

	if (!*list)
		*list = new;
	else
	{
		last = *list;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

t_redirection	*add_redirection(t_redirection **list,
	t_token_type type, t_token_word *word)
{
	t_redirection	*new;

	new = malloc(sizeof(t_redirection));
	if (!new)
		return (NULL);
	new->type = type;
	new->file = join_token_words(word);
	if (!new->file)
		return (free(new), NULL);
	new->fd = -1;
	new->next = NULL;
	append_redir(list, new);
	return (new);
}

int	arg_count(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}
