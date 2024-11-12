/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:53:38 by saoun             #+#    #+#             */
/*   Updated: 2024/11/12 16:06:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Function to validate environment variable names
#include "mini_shell.h"

int	is_valid_var_name(const char *var)
{
	size_t	i;

	i = 0;
	if (!var || !*var || isdigit(var[0]) || var[0] == '=')
		return (0);
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	find_env_var(t_env *myenv, const char *name)
{
	size_t	name_len;
	int i;

	i = 0;
	name_len = ft_strlen(name);
	while (myenv->env[i])
	{
		if (ft_strncmp(myenv->env[i], name, name_len) == 0)
			return (i);
		i++;
	}
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

int	builtin_unset(char **input, t_env *myenv)
{
	int	i;
	int	exit_code;
	int	index;

	i = 0;
	exit_code = 0;
	if (!input[0])
		return (printf("exiting.\n"),exit_code);
	while (input[i])
	{
		if (!is_valid_var_name(input[i]))
		{
			printf("unset: `%s': not a valid identifier\n", input[i]);
			exit_code = 1;
		}
		else
		{
			index = find_env_var(myenv, input[i]);
			if (index != -1)
				remove_env_var(myenv, index);
		}
		i++;
	}
	return (exit_code);
}

void	print_env(t_env *myenv)
{
	int	i;

	i = 0;
	while (myenv->env[i])
	{
		printf("%s\n", myenv->env[i]);
		i++;
	}
}
