/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handling_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:04:01 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/21 13:13:32 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_all_spaces(const char *str)
{
	if (str == NULL)
		return (0);
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

void	add_to_input_args(char *value, t_parser *curr)
{
	curr->input = add_string_to_2d_array(curr->input, value);
	curr->args = add_string_to_2d_array(curr->args, value);
}

int	handle_parsing_redirection_helper(t_input *tokens, t_parser *curr)
{
	if (tokens->type == T_HEREDOC && (tokens->next->type == T_ENV
			|| tokens->next->type == T_IDENTIFIER
			|| tokens->next->type == T_QUOTE))
	{
		curr->delimeter = add_string_to_2d_array(curr->delimeter,
				tokens->next->value);
		add_to_array(curr, T_HEREDOC);
	}
	else if (tokens->type == T_INPUT && (tokens->next->type == T_IDENTIFIER
			|| tokens->next->type == T_PATH || tokens->next->type == T_QUOTE))
	{
		curr->infile = add_string_to_2d_array(curr->infile,
				tokens->next->value);
		add_to_array(curr, T_INPUT);
	}
	else if (tokens->type == T_OUTPUT && (tokens->next->type == T_IDENTIFIER
			|| tokens->next->type == T_PATH || tokens->next->type == T_ENV
			|| tokens->next->type == T_QUOTE))
	{
		curr->outfile = add_string_to_2d_array(curr->outfile,
				tokens->next->value);
		add_to_array(curr, T_OUTPUT);
	}
	return (0);
}

int	handle_parsing_redirection(t_input *tokens, t_parser *curr, t_env *env)
{
	if (tokens->next != NULL && (tokens->next->type == tokens->type
			|| tokens->next->type == T_INPUT || tokens->next->type == T_OUTPUT
			|| tokens->next->type == T_HEREDOC))
	{
		printf("bash: syntax error near unexpected token `%s'\n",
			tokens->value);
		env->exit_code = 2;
		return (g_v = 0, -1);
	}
	if (tokens->next == NULL)
	{
		printf("bash: syntax error near unexpected token `newline`\n");
		env->exit_code = 2;
		return (g_v = 0, -1);
	}
	else if (tokens->type == T_APPEND && (tokens->next->type == T_IDENTIFIER
			|| tokens->next->type == T_PATH || tokens->next->type == T_QUOTE))
	{
		curr->outfile = add_string_to_2d_array(curr->outfile,
				tokens->next->value);
		add_to_array(curr, T_APPEND);
	}
	else
		handle_parsing_redirection_helper(tokens, curr);
	return (0);
}

int	is_builtin_command(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "unset"));
}
