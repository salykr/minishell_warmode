/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 12:27:34 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 13:59:13 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	process_special_variable(char **input)
{
	if (**input >= '0' && **input <= '9')
	{
		(*input)++;
		while (**input && (ft_isalnum(**input) || is_special_char(**input, 1)))
		{
			printf("%c", **input);
			(*input)++;
		}
		return ;
	}
	else if (is_special_char(**input, 0))
	{
		printf("$");
		while (**input && (ft_isalnum(**input) || is_special_char(**input, 1)))
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
			value = ft_strtrim(value, " ");
			printf("%s", value);
			free(value);
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
			if (g_v == 0)
				printf("%d", env.exit_code);
			else
				printf("%d", g_v);
			(*input)++;
			return ;
		}
		else if ((**input >= '0' && **input <= '9')
			|| is_special_char(**input, 0))
			process_special_variable(input);
		else
			process_expanded_variable(input, env);
		return ;
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
