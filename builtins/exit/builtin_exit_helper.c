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

int	handle_exit_arguments(char *input)
{
	if (!is_numeric(input))
	{
		printf("minishell : exit: %s: numeric argument required\n", input);
		g_v = 2;
	}
	else
	{
		printf("minishell: exit: too many arguments\n");
		g_v = 1;
		return (1);
	}
	return (0);
}
