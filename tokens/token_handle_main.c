/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:16:23 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 14:35:31 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./mini_shell.h"

void	handle_braces(const char **p, t_tokenlist *token_list)
{
	char	brace_str[2];
	t_input	*token;

	brace_str[0] = **p;
	brace_str[0] = '\0';
	token = create_token(T_BRACES, brace_str);
	add_token(token_list, token);
	(*p)++;
}

void	handle_quote_identifier(const char **p)
{
	char	quote_char;
	char	*end;

	if (**p == '\\')
	{
		(*p)++;
		quote_char = **p;
		(*p)++;
	}
	else
	{
		quote_char = **p;
		(*p)++;
	}
	end = ft_find_str(p, quote_char);
	*p = end;
}

void	handle_identifier(const char **p, t_tokenlist *token_list)
{
	const char	*start = *p;
	char		*identifier_str;
	t_input		*token;

	while (!ft_isspace(**p) && **p != '\0' && **p != '>' && **p != '<'
		&& ft_strncmp(*p, ">>", 2) && ft_strncmp(*p, "<<", 2) && **p != '|')
	{
		if (**p == '"' || **p == '\'' || (**p == '\\' && (**(p + 1) == '"'
					|| **(p + 1) == '\'')))
			handle_quote_identifier(p);
		(*p)++;
	}
	identifier_str = ft_strndup(start, *p - start);
	token = create_token(T_IDENTIFIER, identifier_str);
	add_token(token_list, token);
	free(identifier_str);
}

void	tokenize_special_input(const char **p, t_tokenlist *token_list)
{
	if ((**p == '-' && ft_isalpha(*(*p + 1))) || ((**p == '-')
			&& (*(*p + 1) == '-') && (ft_isalpha(*(*p + 2)))))
		handle_argument(p, token_list);
	else if ((**p == '/' && ft_isalpha(*(*p + 1))) || (**p == '.' && *(*p
				+ 1) == '/'))
		handle_path(p, token_list);
	else if (((**p == '~' && *(*p + 1) == '/') && ft_isalpha(*(*p + 2))))
		handle_path(p, token_list);
	else if (**p == '~')
		handle_tilde(p, token_list);
	else if (**p == '$')
		handle_env_variable(p, token_list);
	else if (**p == '(' || **p == ')')
		handle_parenthesis(p, token_list);
	else if (**p == '"' || **p == '\'' || (**p == '\\' && (*(*p + 1) == '"'
				|| *(*p + 1) == '\'')))
		handle_quote(p, token_list);
	else if (**p == '{' || **p == '}')
		handle_braces(p, token_list);
	else if (**p != '\0')
		handle_identifier(p, token_list);
}

t_tokenlist	*tokenize_input(const char *input)
{
	const char	*p = input;
	t_tokenlist	*token_list;

	token_list = create_token_list();
	while (*p != '\0')
	{
		while (ft_isspace(*p))
			p++;
		if (((*p == '\'' && *(p + 1) == '\'') || (*p == '\"' && *(p
						+ 1) == '\"')) && ft_isalpha(*(p + 2)))
		{
			p += 2;
			continue ;
		}
		if (*p == '|')
			handle_pipe(&p, token_list);
		else if (*p == '<' || *p == '>')
			handle_redirection(&p, token_list);
		else if (ft_isdigit(*p))
			handle_number(&p, token_list);
		else
			tokenize_special_input(&p, token_list);
	}
	return (token_list);
}
