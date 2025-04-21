/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:54:48 by isalayan          #+#    #+#             */
/*   Updated: 2024/10/09 18:40:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*compare_input_with_str(t_parser *list, const char *str)
{
	int	i;

	if (list == NULL || list->input == NULL || str == NULL)
		return (NULL);
	i = 0;
	while (list->input[i] != NULL)
	{
		if (ft_strcmp(list->input[i], str) != 0)
			return (list->input[i]);
		i++;
	}
	return (NULL);
}

void	update_shlvl(t_env *myenv)
{
	char	*prev_level;
	int		temp;
	char	*new_level_str;

	prev_level = ft_getenv(myenv, "SHLVL");
	if (prev_level && prev_level[0] != '\0')
	{
		temp = ft_atoi(prev_level);
		temp += 1;
		new_level_str = ft_itoa(temp);
		add_or_update_to_env("SHLVL", new_level_str, myenv);
		free(new_level_str);
	}
	else
		add_or_update_to_env("SHLVL", "1", myenv);
}

int	exists_in_env(char *var_name, t_env *myenv)
{
	int	i;

	i = 0;
	while (myenv->env[i] != NULL)
	{
		if (ft_strncmp(myenv->env[i], var_name, ft_strlen(var_name)) == 0
			&& myenv->env[i][ft_strlen(var_name)] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	print_user_defined_vars(t_parser *list, t_env *myenv)
{
	int		j;
	char	*equal_sign;
	char	*var_name;

	j = 0;
	while (list->input[j] != NULL)
	{
		equal_sign = ft_strchr(list->input[j], '=');
		if (equal_sign != NULL)
		{
			var_name = ft_strndup(list->input[j], equal_sign - list->input[j]);
			if (!exists_in_env(var_name, myenv))
				printf("%s\n", list->input[j]);
			free(var_name);
		}
		j++;
	}
}

int	builtin_env(t_parser *list, t_env *myenv)
{
	char	*mismatch_str;

	if (check_ls_pwd_in_env(list, myenv) == 0)
		return (0);
	mismatch_str = check_env_input(list);
	if (mismatch_str != NULL)
	{
		ft_putendl_fd("env:  No such file or directory", 2);
		return (1);
	}
	if (list->input == NULL)
	{
		print_all_env_vars(myenv);
		return (0);
	}
	print_env_vars(myenv, list->input);
	print_user_defined_vars(list, myenv);
	return (0);
}
