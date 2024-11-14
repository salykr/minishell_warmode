/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 20:09:14 by marvin            #+#    #+#             */
/*   Updated: 2024/11/12 20:09:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"


void update_pwd(t_env *myenv)
{
    char cwd[2048];
    char *oldpwd;
    char* value;

    oldpwd = ft_getenv(myenv, "PWD");
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return;
    }
    if (oldpwd)
    {
        value = ft_strdup(oldpwd);
        add_or_update_to_env("OLDPWD",value , myenv);
        free(value);
    }
    value = ft_strdup(cwd);
    add_or_update_to_env("PWD", value, myenv);
    free(value);
}

int	change_directory_and_update(t_parser *list, t_env *myenv)
{
	if (!cmd_is_dir(list->input[0]) || chdir(list->input[0]) != 0)
	{
		printf("path: %s\n",list->input[0]);
		perror("cd");
		global_var = 1;
		return (-1);
	}
	update_pwd(myenv);
	return (0);
}


void replace_with_str(char ***array, char *new_str)
{
	size_t i;

    if (!array || !new_str)
        return;
    if (*array != NULL)
    {
        i = 0;
        while ((*array)[i] != NULL)
        {
            free((*array)[i]);
            i++;
        }
        free(*array);
    }
    *array = (char **)malloc(sizeof(char *) * 2);
    if (*array == NULL)
        return; 
    (*array)[0] = ft_strdup(new_str);
    if ((*array)[0] == NULL)
    {
        free(*array);
        return;
    }
    (*array)[1] = NULL;
}


int is_oldpwd_input(const char *input)
{
	return (ft_strcmp(input, "-") == 0);
}
char *remove_quotes_with_free(char *str)
{
    char *result;
    int j;
    int i;
    bool in_single = false;
    bool in_double = false;

    result = malloc(ft_strlen(str) + 1);
    j = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '"' && !in_single)
            in_double = !in_double;
        else
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    free(str);
    return result;
}