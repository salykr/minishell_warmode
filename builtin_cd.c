/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:36:13 by skreik            #+#    #+#             */
/*   Updated: 2024/12/05 12:57:12 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char *expand_path(t_env *env, char *path)
{
    char *env_var;
    size_t skip_len;
	char *full_path;

	env_var = NULL;
	skip_len = 0;
    if (ft_strncmp(path, "$PWD", 4) == 0)
    {
        env_var = ft_getenv(env, "PWD");
        skip_len = 4;
    }
    else if (ft_strncmp(path, "$HOME", 5) == 0)
    {
        env_var = ft_getenv(env, "HOME");
        skip_len = 5;
    }
    if (env_var == NULL)
        return (NULL);
    full_path = malloc(ft_strlen(env_var) + ft_strlen(path + skip_len) + 1);
    if (full_path == NULL)
        return (NULL);
    ft_strcpy(full_path, env_var);
    ft_strcat(full_path, path + skip_len);
    return (full_path);
}


int is_home_input(char **input)
{
	return (
		input == NULL || ft_strcmp(*input, "") == 0 ||
		ft_strcmp(*input, "$HOME") == 0 || ft_strcmp(*input, "~") == 0 ||
		ft_strcmp(*input, "~/") == 0 || ft_strcmp(*input, "--") == 0
	);
}

int replace_with_env_var(char ***input, t_env *env, char *var_name)
{
	char *env_value = ft_getenv(env, var_name);
	if (env_value == NULL)
	{
		perror("cd");
		return (0);
	}
	printf("the val is: %s \n",env_value);
	replace_with_str(input, env_value);
	return (1);
}



int	handle_directory_input(t_parser *list, t_env *myenv)
{
	char *val;

	val = NULL;
	if (is_home_input(list->input))
	{
		if (!replace_with_env_var(&list->input, myenv, "HOME"))
			return (-1);
	}
	else if (ft_strnstr(list->input[0], "$HOME", ft_strlen(list->input[0])) != NULL)
	{
		val = process_variable(list->input[0], myenv);
		replace_with_str(&list->input, val);
	}	
	else if (is_oldpwd_input(list->input[0]))
	{
		if (!replace_with_env_var(&list->input, myenv, "OLDPWD"))
			return (-1);
		printf("path: %s\n", list->input[0]);
	}
	else if (ft_strncmp(list->input[0], "$PWD", 4) == 0 || ft_strncmp(list->input[0], "$HOME", 5) == 0)
	{
		val = process_variable(list->input[0], myenv);
		replace_with_str(&list->input, val);
	}
	else if(ft_strchr(list->input[0],'$')!=NULL)
	{
		val = process_variable(list->input[0], myenv);
		replace_with_str(&list->input, val);
	}
	if (val != NULL)
		free(val);
	return (0);
}
//-- oldpwd
int	builtin_cd(t_parser *list, t_env *myenv)
{
	printf("list had:\n");
	print_2d_array(list->input);
	//i added these two conditions incase sar she
	if (list->input != NULL && (strcmp(list->input[0],"---")==0))
	{
		printf("cd : Invalid option.\n");
		return (2);
	}
	if (list->input != NULL && (list->input[1] != NULL || strcmp(list->input[0],"*")==0))
	{
		printf("cd :too many arguments.\n");
		return (1);
	}
	if (list->input != NULL && (strchr(list->input[0],'\'') ||strchr(list->input[0],'"')))
		list->input[0]=remove_quotes_with_free(list->input[0]);
	if (list->input != NULL &&(list->input[1] != NULL || !ft_strncmp(list->input[0], "/home", 5)))
        return (1);
	if (handle_directory_input(list, myenv) != 0)
		return (1);
	printf("list now has:\n");
	print_2d_array(list->input);
	return (change_directory_and_update(list, myenv));
}

/*
in this case:
Mkdir -p a/b/c/d
Cd a/b/c/d

Rm -rf ../../../../a
Pwd
Cd ..
Pwd 
Cd ..
Echo $OLDPWD
Echo $PWD
Cd ..

how is the folder becoming ..? evenm tho th prev one is deleted?
*/