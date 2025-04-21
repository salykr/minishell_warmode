/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:36:13 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/19 11:59:47 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_home_input(char **input)
{
	return (
		input == NULL || ft_strcmp(*input, "") == 0
		|| ft_strcmp(*input, "$HOME") == 0 || ft_strcmp(*input, "~") == 0
		|| ft_strcmp(*input, "~/") == 0 || ft_strcmp(*input, "--") == 0);
}

int	replace_with_env_var(char ***input, t_env *env, char *var_name)
{
	char	*env_value;

	env_value = ft_getenv(env, var_name);
	if (env_value == NULL)
	{
		perror("cd");
		return (0);
	}
	replace_with_str(input, env_value);
	return (1);
}

int	handle_input(t_parser *list, t_env *myenv)
{
	if (is_home_input(list->input))
	{
		if (!replace_with_env_var(&list->input, myenv, "HOME"))
			return (-1);
	}
	else if (is_oldpwd_input(list->input[0]))
	{
		if (!replace_with_env_var(&list->input, myenv, "OLDPWD"))
			return (-1);
	}
	return (0);
}

int	handle_directory_input(t_parser *list, t_env *myenv)
{
	char	*val;

	val = NULL;
	if (is_home_input(list->input) || is_oldpwd_input(list->input[0]))
		return (handle_input(list, myenv));
	else if (ft_strncmp(list->input[0], "$PWD", 4) == 0
		|| ft_strchr(list->input[0], '$') != NULL
		|| ft_strrchr(list->input[0], '~') != NULL
		|| ft_strnstr(list->input[0], "$HOME",
			ft_strlen(list->input[0])) != NULL)
	{
		if (ft_strrchr(list->input[0], '~') != NULL)
		{
			val = list->input[0];
			list->input[0] = ft_strjoin("$HOME", list->input[0] + 1);
			free(val);
		}
		val = process_variable(list->input[0], myenv);
		replace_with_str(&list->input, val);
	}
	if (val != NULL)
		free(val);
	return (0);
}

int	builtin_cd(t_parser *list, t_env *myenv)
{
	if (list->input != NULL && (ft_strlen(list->input[0]) > 2
			&& list->input[0][2] == '-'))
	{
		ft_putendl_fd("Invalid option", 2);
		return (2);
	}
	if (list->input != NULL && (list->input[1] != NULL
			|| ft_strcmp(list->input[0], "*") == 0))
	{
		ft_putendl_fd(" too many arguments", 2);
		return (1);
	}
	if (list->input != NULL && (ft_strchr(list->input[0], '\'')
			|| ft_strchr(list->input[0], '"')))
		list->input[0] = remove_quotes_with_free(list->input[0]);
	if (list->input != NULL && (list->input[1] != NULL))
		return (1);
	if (handle_directory_input(list, myenv) != 0)
		return (1);
	return (change_directory_and_update(list, myenv));
}
