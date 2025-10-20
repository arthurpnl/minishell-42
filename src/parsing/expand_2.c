/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:57:40 by mehdi             #+#    #+#             */
/*   Updated: 2025/10/20 13:07:43 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*expand_status(char *res, int status, int *i)
{
	char	*buf;
	char	*joined;

	buf = ft_itoa(status);
	*i += 2;
	joined = ft_strjoin(res, buf);
	free(buf);
	return (joined);
}
