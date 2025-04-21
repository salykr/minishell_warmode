/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:20:53 by isalayan          #+#    #+#             */
/*   Updated: 2025/01/02 17:32:21 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	handle_exit_input(t_parser *parser)
{
	if (parser->input != NULL && parser->input[0] != NULL)
		parser->input[0] = remove_paired_quotes(parser->input[0]);
}

int	wrap_to_byte(int n)
{
	while (n < 0)
		n += 256;
	while (n >= 256)
		n -= 256;
	return (n);
}

void	validate_numeric_input(char *input, t_env *env)
{
	char		*endptr;
	long long	num;

	errno = 0;
	num = ft_strtoll(input, &endptr, 10);
	if (is_numeric(input))
	{
		if (*endptr != '\0' || errno == ERANGE)
		{
			ft_putendl_fd("minishell : exit: numeric argument required", 2);
			env->exit_code = 2;
		}
		else
		{
			num = wrap_to_byte(num);
			env->exit_code = (int)num;
		}
		g_v = 0;
	}
	else
	{
		ft_putendl_fd("minishell : exit: numeric argument required", 2);
		env->exit_code = 2;
		g_v = 0;
	}
}

void	cleanup_and_exit(t_env *myenv, t_parser *parser)
{
	int	exit_code;

	exit_code = myenv->exit_code;
	ft_free_env(&myenv);
	free_parser(parser);
	exit(exit_code);
}

void	builtin_exit(t_parser *parser, t_env *myenv)
{
	if (parser->next == NULL && parser->prev == NULL)
		printf("exit\n");
	handle_exit_input(parser);
	if (parser->input == NULL)
	{
		myenv->exit_code = 0;
		g_v = 0;
		cleanup_and_exit(myenv, parser);
	}
	else if (parser->input[1])
	{
		if (handle_exit_arguments(parser->input, myenv) != 1)
			cleanup_and_exit(myenv, parser);
	}
	else
	{
		validate_numeric_input(parser->input[0], myenv);
		cleanup_and_exit(myenv, parser);
	}
}
