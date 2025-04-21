/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:09:12 by marvin            #+#    #+#             */
/*   Updated: 2024/11/12 17:09:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_numeric(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	handle_exit_argument(char *input, t_env *env)
{
	if (!is_numeric(input))
	{
		ft_putendl_fd("minishell : exit: numeric argument required", 2);
		env->exit_code = 2;
		g_v = 0;
	}
	else
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		env->exit_code = 1;
		g_v = 0;
		return (1);
	}
	return (0);
}

int	handle_exit_arguments(char **input, t_env *env)
{
	int	i;

	env->exit_code = 0;
	i = 0;
	while (input[i] != NULL)
	{
		if (!is_numeric(input[i]))
		{
			ft_putendl_fd("minishell :exit :numeric argument required", 2);
			env->exit_code = 2;
			g_v = 0;
			return (2);
		}
		else
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			env->exit_code = 1;
			g_v = 0;
			return (1);
		}
		i++;
	}
	return (0);
}
