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

/*
int find_and_update_env(int check_input_status, char *new_name, char *new_value, t_env *env)
{
    int i;
    size_t name_len;

    i = 0;
    name_len = strlen(new_name);
    while (env->env[i] != NULL)
    {
        if (strncmp(env->env[i], new_name, strlen(new_name)) == 0 &&
            (env->env[i][strlen(new_name)] == '=' || env->env[i][strlen(new_name)] == '\0'))
            {
                if (check_input_status == -1)
                {
                    char *current_value = strchr(env->env[i], '=');
                    if (current_value) {
                        current_value++;  // Move past the '='
                        char *updated_value = concatenate_value(current_value, new_value);
                        if (updated_value)
                        {
                            free(env->env[i]);
                            env->env[i] = malloc(strlen(new_name) + strlen(updated_value) + 2);
                            if (!env->env[i])
                            {
                                free_name_and_value(new_name, new_value);
                                free(updated_value);
                                return (1);  // Memory allocation error
                            }
                            strcpy(env->env[i], new_name);
                            strcat(env->env[i], "=");
                            strcat(env->env[i], updated_value);
                            free(updated_value);
                        }
                    }
                    else
                    {
                        size_t new_len = name_len + (new_value ? strlen(new_value) : 0) + 1;
                        env->env[i] = malloc(new_len + 2);
                        if (!env->env[i])
                        {
                            free_name_and_value(new_name, new_value);
                            return (1);  // Memory allocation error
                        }
                        if (new_value)
                            snprintf(env->env[i], new_len + 2, "%s=%s", new_name, new_value);
                        else
                            snprintf(env->env[i], new_len + 2, "%s=", new_name);
                        free_name_and_value(new_name, new_value);
                        return (1);  // Found and updated, return 1
                    }
                }
                else
                    replace_or_append_value(&env->env[i], new_name, new_value);
                handle_memory_errors(new_name, new_value);
                return (1);  // Found and updated, return 1
        }
        i++;
    }
    return (0);
}
*/

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
    size_t new_len = strlen(new_name) + (new_value ? strlen(new_value) : 0) + 1;

    *env_var = malloc(new_len + 2);
    if (!(*env_var))
        return 1;  // Memory allocation error

    if (new_value)
        snprintf(*env_var, new_len + 2, "%s=%s", new_name, new_value);
    else
        snprintf(*env_var, new_len + 2, "%s=", new_name);

    return 1;  // Found and updated, return 1
}
*/
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
        return (update_existing_value(env_var, new_name, new_value, current_value));
    //can have replace or append instead?
    // return (allocate_and_update_new_value(env_var, new_name, new_value));
    replace_or_append_value(env_var, new_name, new_value);
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
                printf("name and value: %s %s\n",new_name,new_value);
                handle_memory_errors(new_name, new_value);
            }
            return 1;  // Found and updated, return 1
        }
        i++;
    }
    return 0;
}
