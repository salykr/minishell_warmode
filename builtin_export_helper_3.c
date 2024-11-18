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
    if(*env_entry != NULL)
        free(*env_entry);
    *env_entry = malloc(new_len + 1);
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
        printf("the len is: %ld and name len is %ld\n",equal_sign - input,strlen(*name));
        *value = ft_strdup(equal_sign + 1);
    }
    else
    {
        *name = ft_strdup(input);
        *value = NULL;  // Set value to NULL if '=' is not present
    }
}
//the above add_or_update to_env will be split into two fts:
int process_name_and_value(char *name, char *value, t_env *env, t_name_value *new_nv)
{
    //size_t len;
    int check_input_status;

    new_nv->new_name = process_variable(name, env); 
    if (value != NULL)
        new_nv->new_value = process_variable(value, env);
    else
        new_nv->new_value = NULL;
    handle_memory_errors(name, value);
    if (!(new_nv->new_name)|| (value && !(new_nv->new_value)))
        return (handle_memory_errors(new_nv->new_name, new_nv->new_value),0);  // Handle allocation error
    check_semicolon(new_nv->new_name, &(new_nv->new_value));
    check_input_status = check_input_end(new_nv->new_name);
    if (check_input_status == 0 || (value != NULL && new_nv->new_value != NULL && !check_value(new_nv->new_value)))
        return (handle_memory_errors(new_nv->new_name, new_nv->new_value),0);  // Invalid input or value check failed
    if ((new_nv->new_name)[strlen(new_nv->new_name) - 1] == '+')
        (new_nv->new_name)[strlen(new_nv->new_name) - 1] = '\0';  // Remove the '+' for proper name matching
    return (check_input_status);
}
int handle_shlvl_case(char *name, char *value, t_name_value *new_nv)
{
    if (strcmp("SHLVL", name) == 0)
    {
        new_nv->new_name = name;
        new_nv->new_value = value;
        return 1;
    }
    return 2;  // Return 0 to indicate it’s not the "SHLVL" case
}

int add_or_update_to_env(char *name, char *value, t_env *env)
{
    t_name_value new_nv;
    char **new_env;
    int check_input_status;    
    int i;

    new_nv.new_name = NULL;
    new_nv.new_value  = NULL;
    check_input_status = handle_shlvl_case(name, value, &new_nv);
    if(check_input_status == 2)
        check_input_status = process_name_and_value(name, value, env, &new_nv);
    if (check_input_status == 0)
        return(1);  // Exit if an error occurred in processing
    if (find_and_update_env(check_input_status, new_nv.new_name, new_nv.new_value, env))
        return(0);
    i = ft_doublecharlen(env);  // Get the length of the environment array
    new_env = realloc(env->env, sizeof(char *) * (i + 2));  // Allocate space for the new variable
    if (!new_env)
        return(handle_memory_errors(new_nv.new_name, new_nv.new_value), 1);
    env->env = new_env;
    replace_or_append_value(&env->env[i], new_nv.new_name, new_nv.new_value);
    env->env[i + 1] = NULL;  // Set the last element to NULL
    return(handle_memory_errors(new_nv.new_name, new_nv.new_value), 0);
}

