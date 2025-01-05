/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdennaou <rdennaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:20:53 by rdennaou          #+#    #+#             */
/*   Updated: 2024/12/23 10:20:53 by rdennaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	handle_exit_input(t_parser *parser)
{
	if (parser->input != NULL && parser->input[0] != NULL)
		parser->input[0] = remove_paired_quotes(parser->input[0]);
	//parser->input[0] = remove_quotes(parser->input[0]);
}

int	adjust_number(int num)
{
	if (num < 0)
		num = (num % 256 + 256) % 256;
	else if (num >= 256)
		num = num % 256;
	return (num);
}

void	validate_numeric_input(char *input)
{
	char		*endptr;
	long long	num;

	num = strtoll(input, &endptr, 10);
	if (is_numeric(input))
	{
		if (*endptr != '\0')
		{
			fprintf(stderr, "minishell: exit: %s:numeric argument required\n",
				input);
			global_var = 2;
		}
		else
		{
			num = adjust_number(num);
			global_var = (int)num;
		}
	}
	else
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			input);
		global_var = 2;
	}
}

void	cleanup_and_exit(t_env *myenv, t_parser *parser)
{
	printf("cleaning up\n");
	ft_free_env(&myenv);
	free_parser(parser);
	exit(global_var);
}

void	builtin_exit(t_parser *parser, t_env *myenv)
{
	printf("exit\n");
	handle_exit_input(parser);
	if (parser->input == NULL)
	{
		global_var = 0;
		cleanup_and_exit(myenv, parser);
	}
	else if (parser->input[1])
		handle_exit_arguments(parser->input[0]);
	else
	{
		validate_numeric_input(parser->input[0]);
		cleanup_and_exit(myenv, parser);
	}
}
