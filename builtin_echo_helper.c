/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdennaou <rdennaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 12:27:34 by rdennaou          #+#    #+#             */
/*   Updated: 2024/12/14 12:38:49 by rdennaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	process_special_variable(char **input)
{
	if (**input >= '0' && **input <= '9')
	{
		(*input)++;
		while (**input && (isalnum(**input) || is_special_char(**input)))
		{
			printf("%c", **input);
			(*input)++;
		}
		return ;
	}
	else if (is_special_char(**input))
	{
		printf("$");
		while (**input && (isalnum(**input) || is_special_char(**input)))
		{
			printf("%c", **input);
			(*input)++;
		}
		return ;
	}
}

void	process_normal_variable(char **input, t_env env)
{
	char	*expanded;

	(*input)--;
	if ((**input == '$') && ((*input)[-1] == '\'' || (*input)[-1] == '\"'))
		(*input)--;
	//printf("expnaded = %s\n", *input); // check what goes to process variable
	expanded = process_variable(*input, &env);
	if (expanded)
	{
		printf("%s", expanded);
		free(expanded);
	}
	(*input) += strlen(*input);
}

void	print_expanded_input(char **input, bool inside_single_quotes, t_env env)
{
	if (**input == '$' && !inside_single_quotes)
	{
		(*input)++;
		if (**input == '\0' || **input == '\"' || **input == ' ')
		{
			printf("$");
			return ;
		}
		else if ((**input >= '0' && **input <= '9') || is_special_char(**input))
		{
			process_special_variable(input);
			return ;
		}
		else
		{
			process_normal_variable(input, env);
			return ;
		}
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
