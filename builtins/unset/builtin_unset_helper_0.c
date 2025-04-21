/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_helper_0.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 12:18:53 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 14:34:59 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_valid_var_name(const char *var)
{
	size_t	i;

	i = 0;
	if (!var || !*var || ft_isdigit(var[0]) || var[0] == '=')
		return (0);
	if (var[0] == '\"' && var[1] == '\"')
		return (1);
	if (!ft_isalpha(var[0]) && var[0] != '_' && var[0] != '$')
		return (0);
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_' && var[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

int	get_len_before_dollar(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (i);
}

char	*get_before_dollar(const char *name)
{
	int		i;
	int		j;
	int		len;
	char	*before_dollar;

	i = 0;
	j = 0;
	len = get_len_before_dollar(name);
	before_dollar = malloc(len + 1);
	if (!before_dollar)
		return (NULL);
	while (name[i] != '\0' && name[i] != '$')
		before_dollar[j++] = name[i++];
	before_dollar[j] = '\0';
	return (before_dollar);
}

int	find_env_var(t_env *myenv, const char *name)
{
	int		i;
	size_t	name_len;
	char	*full_name;

	if (!myenv || !name)
		return (-1);
	full_name = process_variable((char *)name, myenv);
	name_len = ft_strlen(full_name);
	i = 0;
	while (myenv->env[i])
	{
		if (ft_strncmp(myenv->env[i], full_name, name_len) == 0
			&& (myenv->env[i][name_len] == '='
			|| myenv->env[i][name_len] == '\0'))
		{
			free(full_name);
			return (i);
		}
		i++;
	}
	free(full_name);
	return (-1);
}

void	remove_env_var(t_env *myenv, int index)
{
	int	i;

	i = index;
	free(myenv->env[i]);
	while (myenv->env[i + 1] != NULL)
	{
		myenv->env[i] = myenv->env[i + 1];
		i++;
	}
	myenv->env[i] = NULL;
}
