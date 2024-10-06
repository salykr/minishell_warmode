/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:36:13 by skreik            #+#    #+#             */
/*   Updated: 2024/10/06 14:49:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"
int list_length(char **list)
{
	int i;

	i = 0;
	while(list[i] != NULL)
		i++;
	return(i);
}
int check_args_nb_1(t_parser *list)
{
	if (list->input == NULL)
		return (1);
	if (list_length(list->input) > 1)
	{
		printf("too many args.\n");
		return (0);
	}
	return(1);	
}

int check_args_nb(t_parser *list)
{
	if (list->input[1])
	{
		printf("too many args.\n");
		return (0);
	}
	return(1);	
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
    (*array)[0] = strdup(new_str); // strdup allocates memory and copies new_str into it
    if ((*array)[0] == NULL)
    {
        free(*array); // Free in case of allocation failure
        return;
    }
    (*array)[1] = NULL;
}

char *expand_path(t_env *env, const char *path)
{
    const char *env_var;
    size_t skip_len;

	env_var = NULL;
	skip_len = 0;
    if (strncmp(path, "$PWD", 4) == 0)
    {
        env_var = ft_getenv(env, "PWD");
        skip_len = 4; // Length of "$PWD"
    }
    else if (strncmp(path, "$HOME", 5) == 0)
    {
        env_var = ft_getenv(env, "HOME");
        skip_len = 5; // Length of "$HOME"
    }
    if (env_var == NULL)
        return (NULL);
    char *full_path = malloc(strlen(env_var) + strlen(path + skip_len) + 1);
    if (full_path == NULL)
        return (NULL);
    strcpy(full_path, env_var);        // Copy environment variable value
    strcat(full_path, path + skip_len); // Append the rest of the path after "$PWD" or "$HOME"
    return (full_path);
}

void update_pwd(t_env *myenv)
{
    char cwd[2048];
    char *oldpwd;

    oldpwd = ft_getenv(myenv, "PWD");
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return;
    }
    if (oldpwd)
        add_or_update_to_env("OLDPWD", strdup(oldpwd), myenv);
    add_or_update_to_env("PWD", strdup(cwd), myenv);
}

// Check if input is related to home directory
int is_home_input(char **input)
{
	printf("HERE HI\n");
	return (
		input == NULL || strcmp(*input, "") == 0 ||
		strcmp(*input, "$HOME") == 0 || strcmp(*input, "~") == 0 ||
		strcmp(*input, "~/") == 0
	);
}

// Check if input is "-"
int is_oldpwd_input(const char *input)
{
	return (strcmp(input, "-") == 0);
}

// Replace input with environment variable value and handle errors
int replace_with_env_var(char ***input, t_env *env, char *var_name, const char *error_msg)
{
	char *env_value = ft_getenv(env, var_name);
	if (env_value == NULL)
	{
		printf("%s", error_msg);
		return (0);
	}
	replace_with_str(input, env_value);
	return (1);
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
int	handle_directory_input(t_parser *list, t_env *myenv)
{
	printf("MADE IT HERE\n");
	if (is_home_input(list->input))
	{
		printf("MADE IT HERE AS WELL\n");
		if (!replace_with_env_var(&list->input, myenv, "HOME", "cd: HOME not set\n"))
			return (-1);
	}
	else if (is_oldpwd_input(list->input[0]))
	{
		if (!replace_with_env_var(&list->input, myenv, "OLDPWD", "cd: OLDPWD not set\n"))
			return (-1);
		printf("path: %s\n", list->input[0]);
	}
	else if (strncmp(list->input[0], "$PWD", 4) == 0 || strncmp(list->input[0], "$HOME", 5) == 0)
		replace_with_str(&list->input, expand_path(myenv, list->input[0]));
	return (0);
}

int	builtin_cd(t_parser *list, t_env *myenv)
{
	printf("PASSED 1\n");
	if (list->input != NULL)
		list->input[0]=remove_quotes(list->input[0]);
	printf("PASSED 2\n");
	// printf("path: %s\n",list->input[0]);
	if (!check_args_nb(list) || (list->input != NULL && !strncmp(list->input[0],"/home",5)))
		return(-1);
	printf("PASSED 3\n");
	if (handle_directory_input(list, myenv) != 0)
		return (-1);
	printf("PASSED 4\n");
	return (change_directory_and_update(list, myenv));
}
