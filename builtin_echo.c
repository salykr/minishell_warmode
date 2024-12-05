/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:55:23 by rdennaou          #+#    #+#             */
/*   Updated: 2024/12/05 13:39:32 by root             ###   ########.fr       */
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
			if (input[i + 1] != '\'')
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
		*arg += strlen(*arg); // Move to the end
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

int	handle_echo_argument(char *arg, t_env *env)
{
	if (!check_balanced_quotes(arg))
	{
		printf("Error: Unbalanced quotes in argument.\n");
		return (0);
	}
	process_argument(arg, env);
	return (1);
}

int	builtin_echo(t_parser *list, t_env *env)
{
	int	i;

	if (!list->input || !list->input[0])
    {
        printf("\n");
		return (1);
    }
	i = 0;
	while (list->input[i])
	{
		if (!handle_echo_argument(list->input[i], env))
			return (1);
		if (list->input[i + 1])
			printf(" ");
		i++;
	}
	if (!list->operations)
		printf("\n");
	return (0);
}

/*
    Things to test again:
    ---------------------
	echo my shit terminal is [$TERM]
	echo $HOME4 (should give nothing)
	echo $HOME% (should return /root%)
    echo \-n hi (-n should work)
	echo $USER$var\$USER$USER\$USERtest$USER
	echo "$HO"ME
    echo "'$HO''ME'"
*/

//119 and 173