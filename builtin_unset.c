/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:53:38 by saoun             #+#    #+#             */
/*   Updated: 2024/12/30 11:52:43 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_valid_var_name(const char *var)
{
	size_t	i;

	i = 0;
	if (!var || !*var || isdigit(var[0]) || var[0] == '=')
		return (0);
	if (var[0] == '\"' && var[1] == '\"')
		return (1);
	if (!isalpha(var[0]) && var[0] != '_' && var[0] != '$')
		return (0);
	while (var[i])
	{
		if (!isalnum(var[i]) && var[i] != '_' && var[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

int	get_len_before_dollar(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (i);
}
char *get_before_dollar(const char *name)
{
	int i = 0, j = 0;
	int len = get_len_before_dollar(name);
	char *before_dollar = malloc(len + 1);
	if (!before_dollar)
		return NULL;
	while (name[i] != '\0' && name[i] != '$')
		before_dollar[j++] = name[i++];
	before_dollar[j] = '\0';
	return before_dollar;
}

char *get_full_env_var_val(t_env *myenv, const char *name)
{
	char *before_dollar = get_before_dollar(name);
	if (!before_dollar)
		return NULL;
	if (name[strlen(before_dollar)] == '$')
	{
		char *after_dollar = get_env_value(myenv, &name[strlen(before_dollar) + 1]);
		if (!after_dollar || after_dollar[0] == '\0')
			return before_dollar;
		char *full_val = ft_str_join(before_dollar, after_dollar);
		free(before_dollar);
		return full_val;
	}
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
	// printf("full_name:%s\n", full_name);
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
	return (-1);
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

int	includes_exlamation_mark(const char *str)
{
	int	i;

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

int handle_unset_options(char **operations, bool *keep_global_var)
{
    if (operations[0] && operations[0][0] == '-') 
	{
        *keep_global_var = true;
        if (operations[0][1] && operations[0][1] == '-' && operations[0][2] != '\0') {
            printf("-bash: unset: --: invalid option\nunset: usage: unset [-f] [-v] [-n] [name ...]\n");
            return 2; // Represents global_var = 2
        } 
		else if (operations[0][1] && operations[0][1] == '-' && operations[0][2] == '\0')
		{
            printf("\n");
            return 0; // Represents global_var = 0
        }
    }
    return -1; // Default return value if no conditions are met
}


int handle_operations_dash(t_parser *parser, bool *keep_global_var, int *dont_enter)
{
    if (parser->operations[0] && parser->operations[0][0] == '-')
    {
        *keep_global_var = true;
        if (parser->operations[0][1] && parser->operations[0][1] == '-' && parser->operations[0][2] != '\0')
        {
            printf("-bash: unset: --: invalid option\nunset: usage: unset [-f] [-v] [-n] [name ...]\n");
            global_var = 2;
            *dont_enter = 1;
        }
        else if (parser->operations[0][1] && parser->operations[0][1] == '-' && parser->operations[0][2] == '\0')
        {
            printf("\n");
            global_var = 0;
            *dont_enter = 1;
        }
        return 1;
    }
    return 0;
}

int handle_operations_flags(t_parser *parser, int *i, int *dont_enter, bool *keep_global_var)
{
    if (parser->operations[0] && parser->operations[0][0] == '-' && *dont_enter == 0)
    {
        *keep_global_var = true;
        while (parser->operations[0][*i] == 'f' || parser->operations[0][*i] == 'v' || parser->operations[0][*i] == 'n')
        {
            (*i)++;
        }
        if (parser->operations[0][*i] != 'f' && parser->operations[0][*i] != 'v' && parser->operations[0][*i] != 'n' && parser->operations[0][*i] != '\0')
        {
            printf("-bash: unset: -%c: invalid option\nunset: usage: unset [-f] [-v] [-n] [name ...]\n", parser->operations[0][*i]);
            global_var = 2;
        }
        else if (parser->operations[0][*i] == '\0')
        {
            printf("\n");
            global_var = 0;
        }
        return 1;
    }
    return 0;
}

int handle_input_dash(t_parser *parser, bool *keep_global_var)
{
    if (parser->input[0] && parser->input[0][0] == '-')
    {
        *keep_global_var = true;
        if (parser->input[0][1] == '\0')
        {
            printf("\n");
            global_var = 0;
        }
        else if (parser->input[0][1] && parser->input[0][1] == '-' 
					&& parser->input[0][2] != '\0')
        {
            printf("-bash: unset: --: invalid option\nunset: usage: unset [-f] [-v] [-n] [name ...]\n");
            global_var = 2;
        }
        else if (parser->input[0][1] && parser->input[0][1] == '-' 
					&& parser->input[0][2] == '\0')
        {
            printf("\n");
            global_var = 0;
        }
        return 1;
    }
    return 0;
}

int validate_variable_name(char *name, bool keep_global_var)
{
    if (!is_valid_var_name(name))
    {
        if (includes_exlamation_mark(name))
        {
            global_var = 0;
            free(name);
            return 1;
        }
        if (!keep_global_var && *name!='\0')
        {
            printf("unset: `%s': not a valid identifier\n", name);
            global_var = 1;
        }
        free(name);
        return 1;
    }
    return 0;
}

void process_variable_removal(t_env *myenv, char *name)
{
    int index;

	index = find_env_var(myenv, name);
    // printf("index:%d\n", index);
    if (index != -1)
    {
        remove_env_var(myenv, index);
        // printf("removing\n");
    }
    free(name);
}

static int process_unset_input(t_parser *parser, t_env *myenv, bool keep_global_var)
{
    char *name;
    int i = 0;

    while (parser->input && parser->input[i])
    {
        name = process_variable(parser->input[i], myenv);
        if (validate_variable_name(name, keep_global_var))
            return global_var;

        process_variable_removal(myenv, name);
        i++;
    }
    return global_var;
}

int builtin_unset(t_parser *parser, t_env *myenv)
{
    int dont_enter = 0;
    bool keep_global_var = false;

    if (!parser || (!parser->input && !parser->operations))
    {
        printf("\n");
        return 0;
    }
    if (parser->operations)
    {
        if (handle_operations_dash(parser, &keep_global_var, &dont_enter))
            return global_var;
        handle_operations_flags(parser, &dont_enter, &dont_enter, &keep_global_var);
    }
    if (parser->input && handle_input_dash(parser, &keep_global_var))
        return global_var;

    return process_unset_input(parser, myenv, keep_global_var);
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
