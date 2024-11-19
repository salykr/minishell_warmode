/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:29:23 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 15:29:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

//replace_or_append_value and concatenate_value

char *resize_string(char *str, size_t new_size)
{
    if (!str || new_size == 0) {
        return NULL;  // Ensure the input is valid
    }
    // Allocate new memory with the specified new size
    char *new_str = (char *)malloc(new_size);
    if (!new_str) {
        free(str);  // Free the old string to prevent leaks
        return NULL;  // Return NULL if memory allocation fails
    }

    // Copy the old content into the new allocated memory
    strncpy(new_str, str, new_size - 1);
    new_str[new_size - 1] = '\0';  // Ensure null termination

    // Free the old memory
    free(str);
    return new_str;  // Return the resized string
}
// //tosgheer th above function
int is_matching_env_var(char *env_var, char *new_name)
{
    size_t name_len = strlen(new_name);
    return (strncmp(env_var, new_name, name_len) == 0 && 
           (env_var[name_len] == '=' || env_var[name_len] == '\0'));
}
/*
int allocate_and_update_new_value(char **env_var, char *new_name, char *new_value)
{
    size_t new_len;

    new_len = strlen(new_name);
    if (new_value)
        new_len += strlen(new_value);
    new_len += 1;  // Add 1 for '=' or '\0'
    *env_var = malloc(new_len + 2);  // +1 for '=' and +1 for '\0'
    if (!(*env_var))
        return 1;  // Memory allocation error

    // Copy new_name and then append '='
    strcpy(*env_var, new_name);
    strcat(*env_var, "=");

    // If new_value exists, append it to env_var
    if (new_value)
        strcat(*env_var, new_value);

    return 1;  // Found and updated, return 1
}
*/

int update_existing_value(char **env_var, char *new_name, char *new_value, char *current_value)
{
    char *updated_value;

    current_value++;  // Move past the '='
    updated_value = concatenate_value(current_value, new_value);
    if (!updated_value)
        return 1;
    if(*env_var != NULL)
        free(*env_var);
    *env_var = malloc(strlen(new_name) + strlen(updated_value) + 2);
    if (!(*env_var))
    {
        free(updated_value);
        return 1;  // Memory allocation error
    }
    strcpy(*env_var, new_name);
    strcat(*env_var, "=");
    strcat(*env_var, updated_value);
    free(updated_value);

    return 1;  // Found and updated, return 1
}

int handle_input_status_negative(char **env_var, char *new_name, char *new_value)
{
    char *current_value;

    current_value = strchr(*env_var, '=');
    if (current_value)
    {
        update_existing_value(env_var, new_name, new_value, current_value);
        handle_memory_errors(new_name,new_value);
        return (1);
    }
    //can have replace or append instead?
    // return (allocate_and_update_new_value(env_var, new_name, new_value));
    replace_or_append_value(env_var, new_name, new_value);
    handle_memory_errors(new_name,new_value);
    return(1);
}

int find_and_update_env(int check_input_status, char *new_name, char *new_value, t_env *env)
{
    int i;

    i = 0;
    while (env->env[i] != NULL)
    {
        if (is_matching_env_var(env->env[i], new_name))
        {
            if (check_input_status == -1)
                return (handle_input_status_negative(&env->env[i], new_name, new_value));
            replace_or_append_value(&env->env[i], new_name, new_value);
            if(strcmp("SHLVL",new_name)!=0)
            {
                // printf("name and value: %s %s\n",new_name,new_value);
                handle_memory_errors(new_name, new_value);
            }
            return 1;  // Found and updated, return 1
        }
        i++;
    }
    return 0;
}
