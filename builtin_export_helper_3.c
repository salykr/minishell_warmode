/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:14:15 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 15:14:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"


char *concatenate_value(char *current_value, char *new_value)
{
    size_t current_len;
    size_t new_len;
    char *updated_value;

    current_len = strlen(current_value);
    if (new_value)
        new_len = current_len + strlen(new_value);
    else
        new_len = current_len;
    updated_value = malloc(new_len + 1);
    if (!updated_value)
        return (NULL);
    ft_strcpy(updated_value, current_value);
    if (new_value)
        ft_strcat(updated_value, new_value);  // Append new_value
    return (updated_value);
}
void replace_or_append_value(char **env_entry, char *new_name, char *new_value) 
{
    size_t name_len;
    size_t new_len;

    name_len = strlen(new_name);
    if (new_value)
        new_len = name_len + strlen(new_value) + 1;
    else
        new_len = name_len + 1;

    // Free the old memory if env_entry is not NULL
    if (*env_entry)
        free(*env_entry);

    *env_entry = malloc(new_len + 1);  // Allocate new memory
    if (!*env_entry)
        return;  // Memory allocation error

    ft_strcpy(*env_entry, new_name);
    if (new_value)
    {
        ft_strcat(*env_entry, "=");
        ft_strcat(*env_entry, new_value);
    }
    else
        ft_strcat(*env_entry, "");
}

void parse_export_input(char *input, char **name, char **value)
{
    char *equal_sign;

    equal_sign = ft_strchr(input, '=');
    *name = NULL;
    *value = NULL;
    if (equal_sign)
    {
        *name = ft_strndup(input, equal_sign - input);
        *value = ft_strdup(equal_sign + 1);
    }
    else
    {
        *name = ft_strdup(input);
        *value = NULL;  // Set value to NULL if '=' is not present
    }
}
//the above add_or_update to_env will be split into two fts:
int process_name_and_value(char *name, char *value, t_env *env, char **new_name, char **new_value)
{
    //size_t len;
    int check_input_status;

    *new_name = process_variable(name, env);    
    if (value != NULL)
        *new_value = process_variable(value, env);
    else
        *new_value = NULL;
    if (!(*new_name) || (value && !(*new_value)))
    {
        handle_memory_errors(*new_name, *new_value);
        return 0;  // Handle allocation error
    }
    check_semicolon(*new_name, new_value);
    check_input_status = check_input_end(*new_name);
    if (check_input_status == 0 || (value != NULL && *new_value != NULL && !check_value(*new_value)))
    {
        handle_memory_errors(*new_name, *new_value);
        return 0;  // Invalid input or value check failed
    }
    if ((*new_name)[strlen(*new_name) - 1] == '+')
        (*new_name)[strlen(*new_name) - 1] = '\0';  // Remove the '+' for proper name matching
    return (check_input_status);
}
/*incase sar ghlt fo2
    if (check_input_status == -1)
    {
        len = strlen(*new_name);
        if ((*new_name)[len - 1] == '+')
            (*new_name)[len - 1] = '\0';  // Remove the '+' for proper name matching
    }
*/


void add_or_update_to_env(char *name, char *value, t_env *env)
{
    char *new_name;
    char *new_value;
    char **new_env;
    int check_input_status;    
    int i;

    new_name = NULL;
    new_value  = NULL;
    check_input_status = process_name_and_value(name, value, env, &new_name, &new_value);
    if (check_input_status == 0)
        return;  // Exit if an error occurred in processing
    if (find_and_update_env(check_input_status, new_name, new_value, env))
        return;
    i = ft_doublecharlen(env);  // Get the length of the environment array
    new_env = realloc(env->env, sizeof(char *) * (i + 2));  // Allocate space for the new variable
    if (!new_env)
    {
        handle_memory_errors(new_name, new_value);
        return;
    }
    env->env = new_env;
    replace_or_append_value(&env->env[i], new_name, new_value);
    env->env[i + 1] = NULL;  // Set the last element to NULL
    handle_memory_errors(new_name, new_value);
}

