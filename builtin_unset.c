/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:53:38 by saoun             #+#    #+#             */
/*   Updated: 2024/12/07 21:42:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Function to validate environment variable names
#include "mini_shell.h"


int is_valid_var_name(const char *var)
{
    size_t i;

    i = 0;
    if (!var || !*var || isdigit(var[0]) || var[0] == '=')
        return (0);
    if (var[0] == '\"' && var[1] == '\"')
        return (1);
    if (!isalpha(var[0]) && var[0] != '_' && var[0] != '$')
        return (0);
    while (var[i])
    {
        if (!isalnum(var[i]) && var[i] != '_' && var[i] != '$' && var[i] != '!')
            return (0);
        i++;
    }
    return (1);
}

int get_len_before_dollar(const char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			return (i);
		}
		i++;
	}
	return (i);
}

char *get_full_env_var_val(t_env *myenv, const char *name)
{
    int i = 0;
    int len = get_len_before_dollar(name);
    char *before_dollar = malloc(len + 1);
    if (!before_dollar)
        return NULL;

    int j = 0;
    while (name[i] != '\0')
    {
        if (name[i] == '$')
        {
            if (name[i + 1] == '\0')
            {
                before_dollar[j] = '\0';
                return before_dollar;
            }
            char *after_dollar = get_env_value(myenv, &name[i]);
            if (!after_dollar || after_dollar[0] == '\0')
			{
                return before_dollar;
			}
            return ft_str_join(before_dollar, after_dollar);
        }
        before_dollar[j++] = name[i++];
    }
    before_dollar[j] = '\0';
    return before_dollar;
}


int	find_env_var(t_env *myenv, const char *name)
{
    int		i;
    size_t	name_len;
    char	*full_name;

    if (!myenv || !name)
        return (-1);
    full_name = process_variable((char*)name, myenv);
	printf("full_name:%s\n", full_name);
    name_len = strlen(full_name);
    i = 0;
    while (myenv->env[i])
    {
        // Check if the current environment variable matches "name" exactly
        if (strncmp(myenv->env[i], full_name, name_len) == 0 &&
            (myenv->env[i][name_len] == '=' || myenv->env[i][name_len] == '\0'))
        {
            free(full_name);
            return (i);
        }
        i++;
    }
    free(full_name);
    return (-1); // Not found
}

void	remove_env_var(t_env *myenv, int index)
{
	int	i;

	i = index;
	// Free the memory of the string at the specified index
	free(myenv->env[i]);
	// Shift remaining elements to the left
	while (myenv->env[i + 1] != NULL)
	{
		myenv->env[i] = myenv->env[i + 1];
		i++;
	}
	// Set the last element to NULL
	myenv->env[i] = NULL;
}

int includes_exlamation_mark(const char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '!' && str[i+1] != '\0')
        {
            printf("bash: !%c event not found\n", (str[i+1]));
            return (1);
        }
        i++;
    }
    return (0);
}

// Function to handle unset operation
int	builtin_unset(t_parser *parser, t_env *myenv)
{
	int	i;
	int	index;
	bool keep_global_var;
    char *name;
    int dont_enter;

    dont_enter = 0;
	i = 0;
	keep_global_var = false;
	if (!parser || (!parser->input && !parser->operations))
	{
		printf("\n");
		return (0);
	}
	if (parser->operations)
	{
		// unset -fnnvnd
		// unset --s
		if (parser->operations[0] && parser->operations[0][0] == '-')
		{
			keep_global_var = true;
			if (parser->operations[0][1] && parser->operations[0][1] == '-' && parser->operations[0][2] != '\0')
			{
				printf("-bash: unset: --: invalid option\nunset: usage: unset [-f] [-v] [-n] [name ...]\n");
				global_var = 2;
                dont_enter = 1;
			}
			else if(parser->operations[0][1] && parser->operations[0][1] == '-' && parser->operations[0][2] == '\0')
			{
				printf("\n");
				global_var = 0;
                dont_enter = 1;
			}
		}
        i = 1;
		if (parser->operations[0] && parser->operations[0][0] == '-' && dont_enter == 0)
		{
			keep_global_var = true;
			while (parser->operations[0][i] == 'f'  || parser->operations[0][i] == 'v' || parser->operations[0][i] == 'n')
			{
				i++;
			}
			if (parser->operations[0][i] != 'f'  && parser->operations[0][i] != 'v' && parser->operations[0][i] != 'n' && parser->operations[0][i] != '\0')
			{
				printf("-bash: unset: -%c: invalid option\nunset: usage: unset [-f] [-v] [-n] [name ...]\n", parser->operations[0][i]);
				global_var = 2;
			}
			else if (parser->operations[0][i] == '\0')
			{
				printf("\n");
				global_var = 0;
			}
		}
	}
	if (parser->input)
	{
		// unset --
		// unset ---s
		if (parser->input[0] && parser->input[0][0] == '-')
		{
			keep_global_var = true;
			if (parser->input[0][1] == '\0')
			{
				printf("\n");
				global_var = 0;
			}
			if (parser->input[0][1] && parser->input[0][1] == '-' && parser->input[0][2] != '\0')
			{
				printf("-bash: unset: --: invalid option\nunset: usage: unset [-f] [-v] [-n] [name ...]\n");
				global_var = 2;
			}
			else if(parser->input[0][1] && parser->input[0][1] == '-' && parser->input[0][2] == '\0')
			{
				printf("\n");
				global_var = 0;
			}
		}
	}
	while (parser->input && parser->input[i])
	{
        name = process_variable(parser->input[i], myenv);
        if (is_valid_var_name(name) && includes_exlamation_mark(name))
        {
            global_var = 0;
            free(name);
            return (global_var);
        }
		if (!is_valid_var_name(name))
		{
            if (includes_exlamation_mark(name))
            {
                global_var = 0;
                free(name);
                return (global_var);
            }
			if (!keep_global_var)
			{
				printf("unset: `%s': not a valid identifier\n", name);
				free(name);
				global_var = 1;
			}
		}
		else
		{
			index = find_env_var(myenv, name);
			printf("\nindex:%d\n", index);
			if (index != -1)
			{
				remove_env_var(myenv, index);
				printf("\nremoving\n");
			}
			if (name)
			{
				free(name);
				name = NULL;
			}
		}
		i++;
	}
	return (global_var);
}

// Function to print the current environment for testing (simulating `env` command)
void	print_env(t_env *myenv)
{
	int	i;

	i = 0;
	while (myenv->env[i])
	{
		printf("%s\n", myenv->env[i]);
		i++;
	}
}



// $> pwd
// $> unset PWD
// $> env | grep PWD
// $> cd $PWD
// $> pwd



