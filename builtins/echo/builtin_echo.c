/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:55:23 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 14:35:48 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_special_char(char c, int i)
{
	if (i == 0)
		return (c == '$' || c == ':' || c == '=' || c == '+' || c == '/'
			|| c == '.' || c == ',' || c == '%' || c == ']' || c == '}');
	else
		return (c == ':' || c == '=' || c == '+' || c == '/'
			|| c == '.' || c == ',' || c == '%' || c == ']' || c == '}');
}

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
			if (input[i + 1] && input[i + 1] != '\'')
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

void	handle_variable_expansion(char **arg, t_env *env)
{
	char	*temp;

	if ((*arg)[1] == '\'' || (*arg)[1] == '\"')
	{
		temp = remove_quotes(*arg);
		printf("%s", temp + 1);
		free(temp);
		*arg += ft_strlen(*arg);
	}
	else
		print_expanded_input(arg, false, *env);
}

void	process_argument(char *arg, t_env *env)
{
	char	quote;

	while (*arg)
	{
		if (*arg == '\'' || *arg == '\"')
		{
			quote = *arg++;
			builtin_echo_helper(&arg, quote, *env);
		}
		else if (*arg == '$')
			handle_variable_expansion(&arg, env);
		else
		{
			if (*arg == '\\')
				arg++;
			printf("%c", *arg++);
		}
	}
}

int	builtin_echo(t_parser *list, t_env *env)
{
	int	i;

	if (!list->input)
	{
		if (!list->operations)
			printf("\n");
		return (0);
	}
	i = 0;
	while (list->input[i])
	{
		if (!check_balanced_quotes(list->input[i]))
		{
			printf("Error: Unbalanced quotes in argument.\n");
			return (1);
		}
		process_argument(list->input[i], env);
		if (list->input[i + 1])
			printf(" ");
		i++;
	}
	if (!list->operations)
		printf("\n");
	return (0);
}
