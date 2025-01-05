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

char	*get_env_value(t_env *env, const char *var) 
{
	if (var[0] == '$') 
		var++; // Skip the '$' character
	if (strcmp(var, "?") == 0) 
		return itoa(env->exit_code); // Convert exit_code to string

	int i = 0;
	char *entry;
	char *equals;
	// char *slash;

	// Iterate through the environment list
	while ((entry = env->env[i]) != NULL) 
	{
		equals = strchr(entry, '=');
		// slash = strchr(entry, '/');
		if (equals)
		{
			// Calculate the length of the variable name
			size_t var_len = equals - entry;
			// Check if the variable name matches
			if (strncmp(entry, var, var_len) == 0 && strlen(var) == var_len)
				return strdup(equals + 1); // Return the value after '='
		}
		// if (slash)
		// {
		//     size_t var_len2 = equals - entry;
		//     printf("%s\n", )
		// }
		i++;
	}
	return (NULL); 
}

//for sorting: print_env_sorted
static int	find_smallest_unprinted(char **env, int *printed, int count)
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

static void	print_env_var(char *env_var)
{
	char	*equal_sign;

	equal_sign = strchr(env_var, '=');
	if (equal_sign)
		printf("declare -x %.*s=\"%s\"\n", (int)(equal_sign - env_var), env_var, equal_sign + 1);
	else
		printf("declare -x %s\n", env_var);
}

// Function to count the number of environment variables and initialize the printed array
static int	count_and_initialize(t_env *env, int **printed)
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

void	print_env_sorted(t_env *env)
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
//sorting fts ended

/*
void print_env_sorted(t_env *env)
{
    if (env == NULL || env->env == NULL)
    {
        fprintf(stderr, "Error: Environment is NULL\n");
        return;
    }
    // Get the number of environment variables
    int count = 0;
    while (env->env[count] != NULL)
        count++;
    // Create an array to track which variables have been printed
    int printed[count];
    int i = 0;
    while (i < count)
    {
        printed[i] = 0; // Initialize all to not printed
        i++;
    }
    int printed_count = 0;
    while (printed_count < count)
    {
        int min_idx = -1;
        int j = 0;

        while (j < count)
        {
            if (!printed[j] && (min_idx == -1 || strcmp(env->env[j], env->env[min_idx]) < 0))
            {
                min_idx = j; // Find the smallest unprinted variable
            }
            j++;
        }
        if (min_idx != -1)
        {
            // Find the position of the '='
            char *equal_sign = strchr(env->env[min_idx], '=');
            if (equal_sign)
            {
                // Print variable with value enclosed in quotes
                printf("declare -x %.*s=\"%s\"\n",
                       (int)(equal_sign - env->env[min_idx]),
                       env->env[min_idx],
                       equal_sign + 1);
            }
            else
            {
                // Print variable without '=' and quotes if no value is present
                printf("declare -x %s\n", env->env[min_idx]);
            }
            printed[min_idx] = 1; // Mark this variable as printed
            printed_count++; // Increment the count of printed variables
        }
    }
}
*/
