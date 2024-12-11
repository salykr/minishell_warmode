/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:33:29 by root              #+#    #+#             */
/*   Updated: 2024/12/11 11:20:08 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_special_char(char c)
{
	return (c == '$' || c == ':' || c == '=' || c == '+');
}

void	print_expanded_input(char **input, bool inside_single_quotes, t_env env)
{
	char	*expanded;

	if (**input == '$' && !inside_single_quotes)
	{
		(*input)++;
		if (**input == '\0' || **input == '\"' || **input == ' ')
		{
			printf("$");
			return ;
		}
		else if (**input >= '0' && **input <= '9')
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
		else
		{
			(*input)--;
			printf("the inut is: %s\n",*input);//sally added this ffor the error
			expanded = process_variable(*input, &env);
			if (expanded)
			{
				printf("%s", expanded);
				free(expanded);
			}
			(*input) += strlen(*input);
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
