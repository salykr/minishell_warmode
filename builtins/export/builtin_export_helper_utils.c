/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitlin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:32:33 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 15:32:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	free_env(t_env *env)
{
	int	i;

	i = 0;
	if (env)
	{
		while (env->env[i])
		{
			free(env->env[i]);
			i++;
		}
		free(env->env);
		free(env);
	}
}

void	handle_memory_errors(char *new_name, char *new_value)
{
	if (new_name != NULL)
		free(new_name);
	if (new_value != NULL)
		free(new_value);
}

int	ft_doublecharlen(t_env *env)
{
	int	i;

	i = 0;
	while (env->env[i] != NULL)
		i++;
	return (i);
}

int	count_quote(const char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '"')
			count++;
		i++;
	}
	return (count);
}

char	*remove_quotes_new_new(const char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = (char *)malloc(sizeof(char)
			*(ft_strlen(str) - count_quote(str)) + 1);
	while (str[i] != '\0')
	{
		if (str[i] != '\'' && str[i] != '"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}
