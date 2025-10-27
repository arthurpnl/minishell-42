/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:42:11 by arthur            #+#    #+#             */
/*   Updated: 2025/10/27 11:30:11 by arpenel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i - 1);
}

void	print_error(char **args)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(args[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static void	handle_exit(t_ctx *ctx, t_commande *cmd_list, int code)
{
	cleanup_and_exit(ctx, cmd_list, code);
	exit(code);
}

int	ft_exit(char **args, t_ctx *ctx, t_commande *cmd_list)
{
	int	arg_count;
	int	exit_code;

	if (!args || !args[0])
		return (1);
	arg_count = count_args(args);
	printf("exit\n");
	if (arg_count == 0)
		handle_exit(ctx, cmd_list, 0);
	if (arg_count == 1)
	{
		if (is_valid_number(args[1]))
		{
			exit_code = ft_atoi(args[1]) % 256;
			handle_exit(ctx, cmd_list, exit_code);
		}
		print_error(args);
		handle_exit(ctx, cmd_list, 2);
	}
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	return (1);
}
