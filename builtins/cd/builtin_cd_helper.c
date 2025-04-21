/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 20:09:14 by marvin            #+#    #+#             */
/*   Updated: 2025/01/12 15:21:49 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	update_pwd(t_parser *list, t_env *myenv)
{
	char	cwd[2048];
	char	*oldpwd;
	char	*value;

	oldpwd = ft_getenv(myenv, "PWD");
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		if (ft_strcmp(list->input[0], "..") == 0 && oldpwd)
		{
			add_or_update_to_env(ft_strdup("OLDPWD"), ft_strdup(oldpwd), myenv);
			oldpwd = ft_strjoin(oldpwd, "/..");
			add_or_update_to_env(ft_strdup("PWD"), oldpwd, myenv);
		}
		return ;
	}
	if (oldpwd)
	{
		value = ft_strdup(oldpwd);
		add_or_update_to_env(ft_strdup("OLDPWD"), value, myenv);
	}
	value = ft_strdup(cwd);
	add_or_update_to_env(ft_strdup("PWD"), value, myenv);
}

int	change_directory_and_update(t_parser *list, t_env *myenv)
{
	char	*home;

	if (list->input != NULL && ((list->input[0] != NULL
				&& list->input[0][0] == '\0') || list->input[0] == NULL))
	{
		home = get_env_value(myenv, "HOME");
		chdir(home);
		free(home);
	}
	else if (!cmd_is_dir(list->input[0]) || chdir(list->input[0]) != 0)
		return (perror("cd"), 1);
	update_pwd(list, myenv);
	return (0);
}

void	clear_array(char ***array)
{
	size_t	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i] != NULL)
	{
		free((*array)[i]);
		i++;
	}
	free(*array);
	*array = NULL;
}

void	allocate_and_assign(char ***array, char *new_str)
{
	*array = (char **)malloc(sizeof(char *) * 2);
	if (*array == NULL)
		return ;
	if (new_str != NULL)
	{
		(*array)[0] = ft_strdup(new_str);
		if ((*array)[0] == NULL)
		{
			free(*array);
			*array = NULL;
			return ;
		}
	}
	else
		(*array)[0] = NULL;
	(*array)[1] = NULL;
}

void	replace_with_str(char ***array, char *new_str)
{
	if (!array)
		return ;
	clear_array(array);
	allocate_and_assign(array, new_str);
}
