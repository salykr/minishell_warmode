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










// void add_or_update_to_env(char *name, char *value, t_env *env)
// {
//     char *new_name;
//     char *new_value;
//     char **new_env;
//     int check_input_status;    
//     int i;

//     new_name = NULL;
//     new_value  = NULL;
//     check_input_status = process_name_and_value(name, value, env, &new_name, &new_value);
//     if (check_input_status == 0)
//         return;  // Exit if an error occurred in processing
//     if (find_and_update_env(check_input_status, new_name, new_value, env))
//         return;
//     i = ft_doublecharlen(env);  // Get the length of the environment array
//     new_env = realloc(env->env, sizeof(char *) * (i + 2));  // Allocate space for the new variable
//     if (!new_env)
//     {
//         handle_memory_errors(new_name, new_value);
//         return;
//     }
//     env->env = new_env;
//     replace_or_append_value(&env->env[i], new_name, new_value);
//     env->env[i + 1] = NULL;  // Set the last element to NULL
//     handle_memory_errors(new_name, new_value);
// }

void add_or_update_to_env(char *name, char *value, t_env *env)
{
    int i = 0;
    int name_len;
    char *str = NULL;
    char *new_value = NULL;
    char *new_name = NULL;
     if (value != NULL)
    {
        if (!check_value(value))
            return; 
    }
    // Check for variables that reference other environment variables in the name
    if (name && strchr(name, '$') && !strstr(name, "\\$"))
    {
        new_name = malloc(strlen(name) + 1);  // +1 for null terminator
        if (!new_name)
            return;

        new_name[0] = '\0';  // Start with an empty string

        char *start = name;
        char *dollar = strchr(name, '$');

        while (dollar)
        {
            // Copy the part before $
            strncat(new_name, start, dollar - start);

            // Get the environment variable's name after $
            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " $");
            if (end_of_var)
            {
                *end_of_var = '\0';  // Temporarily end the string to isolate the variable name
                start = end_of_var;
            }
            else
            {
                start = var_name + strlen(var_name);
            }

            // Fetch the value of the environment variable
            str = get_env_value(env, var_name);
            if (str)
            {
                strcat(new_name, str);
            }

            // Move to the next part of the string
            dollar = strchr(start, '$');
        }

        // Copy any remaining part of the string after the last $
        strcat(new_name, start);

        // Replace name with new_name
        name = new_name;
    }

    name_len = strlen(name);

    // Check for variables that reference other environment variables in the value
    if (value!=NULL && strchr(value, '$') && !strstr(value, "\\$"))
    {
        new_value = malloc(strlen(value) + 1);  // +1 for null terminator
        if (!new_value)
            return;

        new_value[0] = '\0';  // Start with an empty string

        char *start = value;
        char *dollar = strchr(value, '$');

        while (dollar)
        {
            // Copy the part before $
            strncat(new_value, start, dollar - start);

            // Get the environment variable's name after $
            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " $");
            if (end_of_var)
            {
                *end_of_var = '\0';  // Temporarily end the string to isolate the variable name
                start = end_of_var;
            }
            else
            {
                start = var_name + strlen(var_name);
            }

            // Fetch the value of the environment variable
            str = get_env_value(env, var_name);
            if (str)
            {
                strcat(new_value, str);
            }

            // Move to the next part of the string
            dollar = strchr(start, '$');
        }

        // Copy any remaining part of the string after the last $
        strcat(new_value, start);

        // Replace value with new_value
        value = new_value;
    }

    if (ft_haschar(name,'\\'))
        name = ft_escape_char(name);
    if(value && ft_haschar(value, '\\'))
        value = ft_escape_char(value);

    // Check if the variable already exists
    while (env->env[i] != NULL)
    {
        if (strncmp(env->env[i], name, name_len) == 0 &&
            (env->env[i][name_len] == '=' || env->env[i][name_len] == '\0'))
        {
            // Variable found, update it
            free(env->env[i]);
            size_t new_len = name_len + (value ? strlen(value) : 0) + 1; // +1 for '=' or '\0'
            env->env[i] = malloc(new_len + 1);  // +1 for null terminator
            if (!env->env[i])
                return;
            if (value)
                snprintf(env->env[i], new_len + 1, "%s=%s", name, value);
            else
                snprintf(env->env[i], new_len + 1, "%s", name);
            free(new_value);  // Free the allocated new_value buffer
            free(new_name);   // Free the allocated new_name buffer
            return;
        }
        i++;
    }
      // Variable not found, add it
    char **new_env = realloc(env->env, sizeof(char *) * (i + 2));
    if (!new_env)
        return;
    env->env = new_env;
    size_t new_len = name_len + (value ? strlen(value) : 0) + 1; // +1 for '=' or '\0'
    env->env[i] = malloc(new_len + 1);  // +1 for null terminator
    if (!env->env[i])
        return;
    if (value)
        snprintf(env->env[i], new_len + 1, "%s=%s", name, value);
    else
        snprintf(env->env[i], new_len + 1, "%s", name);
    env->env[i + 1] = NULL;

    free(new_value);  // Free the allocated new_value buffer
    free(new_name);   // Free the allocated new_name buffer
}
