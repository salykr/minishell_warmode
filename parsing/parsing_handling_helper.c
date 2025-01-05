/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handling_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:04:01 by isalayan          #+#    #+#             */
/*   Updated: 2025/01/05 17:05:14 by isalayan         ###   ########.fr       */
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
			|| tokens->next->type == T_PATH))
	{
		curr->infile = add_string_to_2d_array(curr->infile,
				tokens->next->value);
		add_to_array(curr, T_INPUT);
	}
	else if (tokens->type == T_OUTPUT && (tokens->next->type == T_IDENTIFIER
			|| tokens->next->type == T_PATH))
	{
		curr->outfile = add_string_to_2d_array(curr->outfile,
				tokens->next->value);
		add_to_array(curr, T_OUTPUT);
	}
	return (0);
}

int	handle_parsing_redirection(t_input *tokens, t_parser *curr)
{
	if ((curr->input == NULL && curr->command == NULL ) ||  (tokens->next!=NULL && (tokens->next->type == tokens->type || tokens->next->type == T_INPUT || tokens->next->type == T_OUTPUT || tokens->next->type == T_HEREDOC)))
	{
		printf("bash: syntax error near unexpected token `%s'\n",
			tokens->value);
		g_v = 2;
		return (-1);
	}
	if (tokens->next == NULL)
	{
		printf("bash: syntax error near unexpected token `newline`\n");
		g_v = 2;
		return (-1);
	}
	else if (tokens->type == T_APPEND && (tokens->next->type == T_IDENTIFIER
			|| tokens->next->type == T_PATH))
	{
		curr->outfile = add_string_to_2d_array(curr->outfile,
				tokens->next->value);
		add_to_array(curr, T_APPEND);
	}
	else
		handle_parsing_redirection_helper(tokens, curr);
	return (0);
}
