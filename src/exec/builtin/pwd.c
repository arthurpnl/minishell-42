/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:14:29 by arthur            #+#    #+#             */
/*   Updated: 2025/10/27 11:25:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args)
{
	char	*pwd_path;
	char	*tmp;

	if (args[1])
	{
		if (args[1][0] == '-')
		{			
			tmp = ft_strjoin_nofree("pwd: ", args[1]);
			print_cmd_error(tmp, ": invalid option\n");
			free(tmp);
			return (2);
		}
	}
	pwd_path = getcwd(NULL, 0);
	if (!pwd_path)
	{
		perror("getcwd");
		return (1);
	}
	printf("%s\n", pwd_path);
	free(pwd_path);
	return (0);
}
