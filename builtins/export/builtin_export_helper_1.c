/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:51:31 by marvin            #+#    #+#             */
/*   Updated: 2025/01/02 16:26:23 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	check_semicolon(char *name, char **value)
{
	char	*semicolon_in_name;
	char	*semicolon_in_value;

	semicolon_in_name = ft_strchr(name, ';');
	if (semicolon_in_name != NULL)
	{
		if (semicolon_in_name != name && *(semicolon_in_name - 1) == '=')
			*(semicolon_in_name - 1) = '\0';
		*semicolon_in_name = '\0';
		if (*value != NULL)
		{
			free(*value);
			*value = NULL;
			return ;
		}
	}
	else if (*value != NULL)
	{
		semicolon_in_value = ft_strchr(*value, ';');
		if (semicolon_in_value != NULL)
			*semicolon_in_value = '\0';
	}
}

int	check_input(char *str)
{
	size_t	i;

	i = 0;
	if (str[0] == '\0' || (!ft_isalpha(str[0]) && (str[0] != '_'
				&& str[0] != '\'' && str[0] != '\"' && str[0] != '$')))
	{
		ft_putendl_fd("Error", 2);
		return (0);
	}
	while (str[++i])
	{
		if (str[i] == '=')
			return (1);
		if (!(ft_isalpha(str[i]) || ft_isdigit(str[i]) || str[i] == '_'
				|| str[i] == '?' || str[i] == '\\' || str[i] == ';'
				|| str[i] == '\'' || str[i] == '\"' || str[i] == '$')
			|| (str[i] == '\\' && str[i + 1] == '\\'))
		{
			if ((i == ft_strlen(str) - 1) && str[ft_strlen(str) - 1] == '+')
				return (-1);
			ft_putendl_fd("Error", 2);
			return (0);
		}
	}
	return (1);
}

int	check_input_end(char *str)
{
	size_t	i;

	i = 0;
	if (str == NULL || str[0] == '\0')
		return ((void)ft_putendl_fd("export: not a valid identifier\n", 2), 0);
	if (str[0] == '\0' || (!ft_isalpha(str[0]) && str[0] != '_'))
	{
		ft_putendl_fd("Error", 2);
		return (0);
	}
	while (str[++i])
	{
		if (!(ft_isalpha(str[i]) || ft_isdigit(str[i]) || str[i] == '_'
				|| str[i] == '\\' || str[i] == ';')
			|| (str[i] == '\\' && str[i + 1] == '\\'))
		{
			if ((i == ft_strlen(str) - 1) && str[ft_strlen(str) - 1] == '+')
				return (-1);
			ft_putendl_fd("Error", 2);
			return (0);
		}
	}
	return (1);
}

int	check_value(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '(' || str[i] == ')' || str[i] == '&'
			|| str[i] == '!' || str[i] == ';')
		{
			ft_putendl_fd("Error.", 2);
			return (0);
		}
		else
			i++;
	}
	return (1);
}

int	ft_haschar(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}
