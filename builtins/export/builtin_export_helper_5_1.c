/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5_1.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:01:07 by marvin            #+#    #+#             */
/*   Updated: 2024/11/18 17:01:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*create_array_till_dollar(char *str, int index)
{
	char	*new_string;
	int		i;

	i = 0;
	new_string = malloc(index + 1);
	while (i < index)
	{
		new_string[i] = str[i];
		i++;
	}
	new_string[index] = '\0';
	return (new_string);
}

int	find_dollar_pos11(char *str, int j)
{
	int	i;

	i = j;
	while (str[i])
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (-42);
}

int	find_end_variable(char *str, int j)
{
	int	i;

	i = find_dollar_pos11(str, j);
	i++;
	if (str[i] == '\"' || str[i] == '\'')
		return (i + 1);
	if (str[i] == '?')
		return (i + 1);
	while (str[i] && ((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= '0' && str[i] <= '9')
			|| str[i] == '_' ))
	{
		i++;
	}
	return (i);
}

int	find_dollar_pos1(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (-42);
}

int	is_num_or_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}
