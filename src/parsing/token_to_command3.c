/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_command3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehdi <mehdi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:33:49 by mehdi             #+#    #+#             */
/*   Updated: 2025/10/08 09:35:35 by mehdi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words(t_token_word *word)
{
	int	count;

	count = 0;
	while (word)
	{
		count++;
		word = word->next;
	}
	return (count);
}

static int	count_non_empty(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
		if (args[i++][0] != '\0')
			count++;
	return (count);
}

static void	copy_non_empty(char **dst, char **src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i][0] != '\0')
			dst[j++] = src[i];
		else
			free(src[i]);
		i++;
	}
	dst[j] = NULL;
}

char	**clean_args(char **args)
{
	int		count;
	char	**new_args;

	if (!args)
		return (NULL);
	count = count_non_empty(args);
	if (count == 0 || args[1] == NULL)
		return (args);
	new_args = malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return (args);
	copy_non_empty(new_args, args);
	free(args);
	return (new_args);
}
