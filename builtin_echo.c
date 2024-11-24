/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:55:23 by rdennaou          #+#    #+#             */
/*   Updated: 2024/11/24 17:11:20 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

bool	check_balanced_quotes(const char *input)
{
	char	quote;
	int		i;

	i = 0;
	quote = '\0';
	while (input[i])
	{
		if (input[i] == '\\')
		{
			if (input[i+1] != '\'')
				i++;
		}
		else if (input[i] == '\'' || input[i] == '\"')
		{
			if (quote == '\0')
				quote = input[i];
			else if (quote == input[i])
				quote = '\0';
		}
		i++;
	}
	return (quote == '\0');
}

int	is_special_char(char c)
{
	return (c == '$' || c == ':' || c == '=' || c == '+');
}

void    print_expanded_input(char **input, bool inside_single_quotes, t_env env)
{
	if (**input == '$' && !inside_single_quotes)
	{
		(*input)++;
		if (**input == '\0' || **input == '\"' || **input == ' ')
		{
			ft_putchar_fd('$', 1);
			return;
		}
		else if (**input >= '0' && **input <= '9')
		{
			(*input)++;
			while (**input && (isalnum(**input) || is_special_char(**input)))
			{
				ft_putchar_fd(**input, 1);
				(*input)++;
			}
			return;
		}
		else if (is_special_char(**input))
		{
			ft_putchar_fd('$', 1);
			while (**input && (isalnum(**input) || is_special_char(**input)))
			{
				ft_putchar_fd(**input, 1);
				(*input)++;
			}
			return;
		}
		else
		{
			(*input)--;  // Go back to the '$'
			char *expanded = process_variable(*input, &env);
			if (expanded)
			{
				printf("%s", expanded);  // Print the expanded variable value
				free(expanded);  // Free the expanded string
			}
			(*input) += strlen(*input);  // Move the pointer past the current input
			return;
		}
	}
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
			if (**input == '\"' || **input == '\\')
				printf("%c", **input);
			else
				printf("\\%c", **input);
			(*input)++;
		}
		else if (**input == '$' && !inside_single_quotes)
		{
			print_expanded_input(input, inside_single_quotes, env);
			continue;
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

void	builtin_echo(t_parser *list, t_env env)
{
	int		i;
	char	*arg;

	i = 0;
	if (!list->input[i])
		return;
	while (list->input[i])
	{
		if (!check_balanced_quotes(list->input[i]))
		{
			printf("Error: Unbalanced quotes in argument %d.\n", i + 1);
			return;
		}
		arg = list->input[i];
		while (*arg)
		{
			if (*arg == '\'' || *arg == '\"')
			{
				char quote = *arg;
				arg++;
				builtin_echo_helper(&arg, quote, env);
			}
			else if (*arg == '$')
			{
				if (*(arg+1) == '\'' || *(arg +1) =='\"')
				{
					arg = remove_quotes(arg);
					printf("%s",arg+1);
					break;
				}
				print_expanded_input(&arg, false, env);
			}
			else
			{
				if (*arg == '\\')
					arg++;
				printf("%c", *arg);
				arg++;
			}
		}
		if (list->input[i + 1])
			printf(" ");
		i++;
	}
	if (list->operations == NULL)
		printf("\n");    
} 


/*
    Things to test again:
    ---------------------
    echo "-n" Hi (the -n should work in this case but instead it's printing -n Hi/ same with echo '-n' Hi)
    echo $?$ (is giving $0 instead of 0$)
    echo \-n hi (-n should work)
*/
