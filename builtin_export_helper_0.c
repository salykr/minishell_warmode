/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fts_print_env_sorted.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:52:38 by marvin            #+#    #+#             */
/*   Updated: 2024/10/18 12:52:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char *get_env_value(t_env *env, const char *var)
{
    if (var[0] == '$') 
        var++; // Skip the '$' character
    if (strcmp(var, "?") == 0) 
        return itoa(env->exit_code); // Convert exit_code to string

    int i = 0;
    char *entry;
    char *equals;

    // Iterate through the environment list
    while ((entry = env->env[i]) != NULL) 
    {
        equals = ft_strchr(entry, '=');
        if (equals)
        {
            // Calculate the length of the variable name
            size_t var_len = equals - entry;
            // Check if the variable name matches
            if (ft_strncmp(entry, var, var_len) == 0 && ft_strlen(var) == var_len)
                return ft_strdup(equals + 1); // Return the value after '='
        }
        i++;
    }
    return(NULL); 
}

//for sorting: print_env_sorted
static int find_smallest_unprinted(char **env, int *printed, int count)
{
    int min_idx;
    int j;

    min_idx = -1;
    j = 0;
    while (j < count)
    {
        if (!printed[j] && (min_idx == -1 || strcmp(env[j], env[min_idx]) < 0))
            min_idx = j;
        j++;
    }
    return (min_idx);
}

static void print_env_var(char *env_var)
{
    char *equal_sign;

    equal_sign = strchr(env_var, '=');
    if (equal_sign)
        printf("declare -x %.*s=\"%s\"\n", (int)(equal_sign - env_var), env_var, equal_sign + 1);
    else
        printf("declare -x %s\n", env_var);
}

// Function to count the number of environment variables and initialize the printed array
static int count_and_initialize(t_env *env, int **printed)
{
    int count;
    int i;

    i = 0;
    count = 0;
    //use ft_doublecharlen
    while (env->env[count])
        count++;
    *printed = malloc(count * sizeof(int));
    if (!*printed)
        return (-1);
    while (i < count)
    {
        (*printed)[i] = 0;
        i++;
    }
    return (count); // Return the count of environment variables
}

void print_env_sorted(t_env *env)
{
    int *printed;
    int count;
    int printed_count;
    int min_idx;

    printed_count = 0;
    count = count_and_initialize(env, &printed);
    if (count == -1)
        return;
    while (printed_count < count)
    {
        min_idx = find_smallest_unprinted(env->env, printed, count);
        print_env_var(env->env[min_idx]);
        printed[min_idx] = 1;
        printed_count++;
    }
    free(printed);
}
