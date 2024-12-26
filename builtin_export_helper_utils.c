/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitlin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:32:33 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 15:32:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"

void free_env(t_env *env)
{
    int i = 0;
    if (env)
    {
        while (env->env[i])
        {
            free(env->env[i]);
            i++;
        }
        free(env->env);
        free(env);
    }
}


void handle_memory_errors(char *new_name, char *new_value)
{
    if(new_name != NULL)
        free(new_name);
    if(new_value != NULL)
        free(new_value);
}


int ft_doublecharlen(t_env *env)
{
    int i;

    i = 0;
    while (env->env[i] != NULL)
        i++;
    return (i);
}

// Function to convert integer to string
char *itoa(int num) 
{
    char buffer[12]; // Buffer to hold the string representation of the integer
    snprintf(buffer, sizeof(buffer), "%d", num);
    return strdup(buffer); // Duplicate the string for return
}

int count_quote(const char *str)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(str[i]!='\0')
    {
        if (str[i]=='\'' || str[i]=='"')
            count++;
        i++;
    }
    return (count);
}
