/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 12:27:34 by rdennaou          #+#    #+#             */
/*   Updated: 2025/01/03 13:14:34 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	process_special_variable(char **input)
{
	if (**input >= '0' && **input <= '9')
	{
		(*input)++;
		while (**input && (ft_isalnum(**input) || is_special_char(**input)))
		{
			printf("%c", **input);
			(*input)++;
		}
		return ;
	}
	else if (is_special_char(**input))
	{
		printf("$");
		while (**input && (ft_isalnum(**input) || is_special_char(**input)))
		{
			printf("%c", **input);
			(*input)++;
		}
		return ;
	}
}

void	process_expanded_variable(char **input, t_env env)
{
	char	*var_name_start;
	size_t	var_name_length;
	char	*var_name;
	char	*value;

	var_name_start = *input;
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	var_name_length = *input - var_name_start;
	if (var_name_length > 0)
	{
		var_name = (char *)malloc(var_name_length + 1);
		ft_strncpy(var_name, var_name_start, var_name_length);
		var_name[var_name_length] = '\0';
		value = ft_getenv(&env, var_name);
		if (value)
		{
			value= ft_strtrim(value, " ");
			printf("%s", value);
		}
		free(var_name);
	}
}

void	print_expanded_input(char **input, bool inside_single_quotes, t_env env)
{
	if (**input == '$' && !inside_single_quotes)
	{
		(*input)++;
		if (**input == '\0' || **input == '\"' || **input == '\''
			|| **input == ' ')
		{
			printf("$");
			return ;
		}
		else if (**input == '?')
		{
			printf("%d", global_var);
			(*input)++;
			return ;
		}
		else if ((**input >= '0' && **input <= '9') || is_special_char(**input))
		{
			process_special_variable(input);
			return ;
		}
		else
			process_expanded_variable(input, env);
	}
}

void	printable(char **input)
{
	if (**input == '\"' || **input == '\\')
		printf("%c", **input);
	else
		printf("\\%c", **input);
}

void	builtin_echo_helper(char **input, char quote, t_env env)
{
	bool	inside_single_quotes;

	inside_single_quotes = (quote == '\'');
	while (**input && **input != quote)
	{
		if (**input == '\\' && !inside_single_quotes)
		{
			(*input)++;
			printable(input);
			(*input)++;
		}
		else if (**input == '$' && !inside_single_quotes)
		{
			print_expanded_input(input, inside_single_quotes, env);
			continue ;
		}
		else
		{
			printf("%c", **input);
			(*input)++;
		}
	}
	if (**input == quote)
		(*input)++;
}
