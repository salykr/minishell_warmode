/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:54:48 by saoun             #+#    #+#             */
/*   Updated: 2024/10/09 16:40:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char *compare_input_with_str(t_parser *list, const char *str)
{
    int i;

    if (list == NULL || list->input == NULL || str == NULL)
        return (NULL);  // Return NULL if list, input, or str is NULL
    i = 0;
    while (list->input[i] != NULL)
    {
        if (strcmp(list->input[i], str) != 0)
            return (list->input[i]);  // Return the mismatched string
        i++;
    }
    return (NULL);  // Return NULL if all strings match
}

char	*ft_getenv(t_env *myenv, char *str)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(str);
	while (myenv->env[i])
	{
		if (!strncmp(myenv->env[i], str, len) && myenv->env[i][len] == '=')
			return (myenv->env[i] + len + 1);
		i++;
	}
	return (NULL);
}
/*
void update_SHLVL(t_env *myenv)
{
    char *prev_level;
    int temp;
    int i;
    char *new_level_str;

    i = 0;
    while (myenv->env[i])
    {
        if (!ft_strncmp(myenv->env[i], "SHLVL=", 6))
        {
            prev_level = myenv->env[i] + 6;  
            if (prev_level[0] != '\0')
            {
                temp = ft_atoi(prev_level); 
                temp += 1;
                new_level_str = ft_itoa(temp); 
                add_or_update_to_env("SHLVL", new_level_str, myenv);
                free(new_level_str);
            }
            return;
        }
        i++;
    }
    add_or_update_to_env("SHLVL", "1", myenv);
}
 */

void update_SHLVL(t_env *myenv)
{
    char *prev_level;
    int temp;
    char *new_level_str;

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
t_env *init_env(char **env)
{
    t_env *myenv;
    int i;
    
    i = 0;
    myenv = (t_env *)malloc(sizeof(t_env));
    if (!myenv)
        return (NULL);
    while (env[i])
        i++;
    myenv->env = (char **)malloc(sizeof(char *) * (i + 1));
    if (!myenv->env)
    {
        free(myenv);
        return (NULL);
    }
    i = 0;
    while (env[i])
    {
        myenv->env[i] = strdup(env[i]);
        i++;
    }
    myenv->env[i] = NULL;
    update_SHLVL(myenv);
    return (myenv);
}

// Helper function to compare env variable name with user input and print the correct value
int match_and_print_env(char *env_var, char **input_list)
{
    int j = 0;
    while (input_list[j] != NULL)
    {
        char *equal_sign = strchr(input_list[j], '=');
        if (equal_sign != NULL)
        {
            // Extract user-provided variable name
            char *var_name = strndup(input_list[j], equal_sign - input_list[j]);

            // Compare env variable name with user-provided variable name
            if (strncmp(env_var, var_name, equal_sign - input_list[j]) == 0 && env_var[equal_sign - input_list[j]] == '=')
            {
                // If names match, print the user-provided value
                printf("%s\n", input_list[j]);
                free(var_name);
                return (1);  // Mark that we found a match and printed
            }
            free(var_name);
        }
        j++;
    }
    return (0);  // No match found
}

// Function to print all environment variables from myenv
void print_all_env_vars(t_env *myenv)
{
    int i = 0;
    while (myenv->env[i] != NULL)
    {
        printf("%s\n", myenv->env[i]);
        i++;
    }
}

// Function to print environment variables and replace matching variables with user input
void print_env_vars(t_env *myenv, char **input_list)
{
    int i = 0;
    while (myenv->env[i] != NULL)
    {
        if (!match_and_print_env(myenv->env[i], input_list))
        {
            // If no match, print the environment variable
            printf("%s\n", myenv->env[i]);
        }
        i++;
    }
}

// Helper function to check if a user-defined variable exists in the environment
int exists_in_env(char *var_name, t_env *myenv)
{
    int i = 0;
    while (myenv->env[i] != NULL)
    {
        if (strncmp(myenv->env[i], var_name, strlen(var_name)) == 0 && myenv->env[i][strlen(var_name)] == '=')
        {
            return (1);  // Exists in environment
        }
        i++;
    }
    return (0);  // Does not exist
}

// Function to print user-defined variables that don't exist in the environment
void print_user_defined_vars(t_parser *list, t_env *myenv)
{
    int j = 0;
    while (list->input[j] != NULL)
    {
        char *equal_sign = strchr(list->input[j], '=');
        if (equal_sign != NULL)
        {
            // Extract user-provided variable name
            char *var_name = strndup(list->input[j], equal_sign - list->input[j]);

            // Check if the variable exists in the environment
            if (!exists_in_env(var_name, myenv))
            {
                // If it doesn't exist, print the user-provided variable
                printf("%s\n", list->input[j]);
            }

            free(var_name);
        }
        j++;
    }
}

// Main builtin_env function that uses the helper functions
int builtin_env(t_parser *list, t_env *myenv)
{
    // If no input is provided, print the entire environment
    if (list->input == NULL)
    {
        print_all_env_vars(myenv);
        return (0);
    }

    print_env_vars(myenv, list->input);          // First pass: Print environment variables
    print_user_defined_vars(list, myenv);        // Second pass: Print user-defined variables that don't exist
    return (0);
}
