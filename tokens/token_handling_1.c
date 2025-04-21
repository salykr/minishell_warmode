/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handling_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:12:39 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/19 12:01:36 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./mini_shell.h"

void	handle_pipe(const char **p, t_tokenlist *token_list)
{
	t_input	*token;

	token = create_token(T_PIPE, "|");
	add_token(token_list, token);
	(*p)++;
}

void	handle_redirection(const char **p, t_tokenlist *token_list)
{
	t_input	*token;

	if (*(*p + 1) == **p)
	{
		if (**p == '<')
			token = create_token(T_HEREDOC, "<<");
		else
			token = create_token(T_APPEND, ">>");
		(*p) += 2;
	}
	else
	{
		if (**p == '<')
			token = create_token(T_INPUT, "<");
		else
			token = create_token(T_OUTPUT, ">");
		(*p)++;
	}
	add_token(token_list, token);
}

void	handle_number(const char **p, t_tokenlist *token_list)
{
	t_input		*token;
	char		*number_str;
	const char	*start = *p;

	while (!ft_isspace(**p) && **p != '\0')
		(*p)++;
	number_str = ft_strndup(start, *p - start);
	token = create_token(T_NUMBER, number_str);
	add_token(token_list, token);
	free(number_str);
}

void	handle_argument(const char **p, t_tokenlist *token_list)
{
	const char	*start = *p;
	char		*argument_str;
	t_input		*token;

	(*p)++;
	while (!ft_isspace(**p) && **p != '|' && **p != '\0')
		(*p)++;
	argument_str = ft_strndup(start, *p - start);
	token = create_token(T_ARGUMENT, argument_str);
	add_token(token_list, token);
	free(argument_str);
}

void	handle_path(const char **p, t_tokenlist *token_list)
{
	const char	*start = *p;
	char		*path_str;
	t_input		*token;

	(*p)++;
	while (**p != '\0' && !ft_isspace(**p) && **p != '|' && **p != '>'
		&& **p != '<' && **p != '(' && **p != ')' && **p != '{' && **p != '}'
		&& **p != '$' && **p != '+' && **p != '-' && **p != '*')
	{
		(*p)++;
	}
	path_str = ft_strndup(start, *p - start);
	token = create_token(T_PATH, path_str);
	add_token(token_list, token);
	free(path_str);
}
