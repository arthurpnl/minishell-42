/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 08:38:58 by mehdi             #+#    #+#             */
/*   Updated: 2025/10/23 16:11:55 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**append_arg(char **args, t_token_word *word)
{
	int		old_size;
	char	**new_args;
	char	*joined;
	int		i;

	if (!word)
		return (args);
	old_size = arg_count(args);
	new_args = malloc(sizeof(char *) * (old_size + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < old_size)
	{
		new_args[i] = args[i];
		i++;
	}
	joined = join_token_words(word);
	if (!joined)
		return (free(new_args), NULL);
	new_args[old_size] = joined;
	new_args[old_size + 1] = NULL;
	free(args);
	return (new_args);
}

static t_commande	*append_command(t_commande **head)
{
	t_commande	*curr;
	t_commande	*last;

	curr = ft_calloc(1, sizeof(t_commande));
	if (!curr)
		return (NULL);
	if (!*head)
		*head = curr;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = curr;
	}
	return (curr);
}

static int	handle_redir(t_commande *curr, t_token **tokens)
{
	if ((*tokens)->next && (*tokens)->next->type == TOK_WORD)
	{
		if (!add_redirection(&curr->redirection,
				(*tokens)->type, (*tokens)->next->word))
			return (0);
		*tokens = (*tokens)->next;
	}
	else
	{
		printf("minishell: missing redirection target\n");
		return (0);
	}
	return (1);
}

static int	handle_token(t_token **tokens, t_commande **curr)
{
	if ((*tokens)->type == TOK_WORD)
		(*curr)->args = append_arg((*curr)->args, (*tokens)->word);
	else if ((*tokens)->type == TOK_REDIR_IN || (*tokens)->type == TOK_REDIR_OUT
		|| (*tokens)->type == TOK_REDIR_APPEND
		|| (*tokens)->type == TOK_HEREDOC)
	{
		if (!handle_redir(*curr, tokens))
			return (0);
	}
	else if ((*tokens)->type == TOK_PIPE)
		*curr = NULL;
	return (1);
}

t_commande	*tokens_to_command(t_token *tokens)
{
	t_commande	*head;
	t_commande	*curr;
	t_commande	*tmp;

	head = NULL;
	curr = NULL;
	while (tokens)
	{
		if (!curr)
		{
			curr = append_command(&head);
			if (!curr)
				return (NULL);
		}
		if (!handle_token(&tokens, &curr))
			return (NULL);
		tokens = tokens->next;
	}
	tmp = head;
	while (tmp)
	{
		tmp->args = clean_args(tmp->args);
		tmp = tmp->next;
	}
	return (head);
}
