/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:51:55 by arpenel           #+#    #+#             */
/*   Updated: 2025/10/24 19:27:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total;

	total = count * size;
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, total);
	return (ptr);
}

void	*ft_memset(void *s, int c, size_t n)
{
	char	*str;

	str = s;
	while (n)
	{
		*str++ = (unsigned char)c;
		n--;
	}
	return (s);
}

void	print_cmd_error(char *cmd, char *error_msg)
{
	char *full_msg;
	char *tmp;

	if (cmd)
	{
		tmp = ft_strjoin_nofree(SHELL_NAME, cmd);
		full_msg = ft_strjoin_nofree(tmp, error_msg);
		free(tmp);
	}
	write(2, full_msg, ft_strlen(full_msg));
	free(full_msg);
}
