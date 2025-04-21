/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handling_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:14:53 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 13:57:09 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./mini_shell.h"

void	handle_tilde(const char **p, t_tokenlist *token_list)
{
	char	tilde_str[3];
	t_input	*token;

	if (*(*p + 1) && (*(*p + 1) == '-' || *(*p + 1) == '$' || *(*p + 1) == '/'))
	{
		tilde_str[0] = **p;
		tilde_str[1] = *(*p + 1);
		tilde_str[2] = '\0';
		(*p)++;
	}
	else
	{
		tilde_str[0] = **p;
		tilde_str[1] = '\0';
	}
	token = create_token(T_TILDE, tilde_str);
	add_token(token_list, token);
	(*p)++;
}

void	handle_env_variable(const char **p, t_tokenlist *token_list)
{
	const char	*start = *p;
	char		*env_str;
	t_input		*token;

	if (**p != '\0')
		(*p)++;
	if (**p == '!' || **p == '@' || **p == '*' || **p == '_')
	{
		if (**p != '\0')
			(*p)++;
	}
	else
	{
		while (!ft_isspace(**p) && **p != '\0')
			(*p)++;
	}
	env_str = ft_strndup(start, *p - start);
	token = create_token(T_ENV, env_str);
	add_token(token_list, token);
	free(env_str);
}

void	handle_parenthesis(const char **p, t_tokenlist *token_list)
{
	char	paren_str[2];
	t_input	*token;

	paren_str[0] = **p;
	paren_str[1] = '\0';
	token = create_token(T_PARENTHESIS, paren_str);
	add_token(token_list, token);
	(*p)++;
}

void	handle_quote(const char **p, t_tokenlist *token_list)
{
	char		quote_char;
	const char	*start;
	char		*end;
	char		*quoted_str;
	t_input		*token;

	if (**p == '\\' && **p != '\0')
	{
		start = *p;
		(*p)++;
		quote_char = **p;
		(*p)++;
	}
	else if (**p != '\0')
	{
		quote_char = **p;
		start = *p;
		(*p)++;
	}
	end = ft_find_str(p, quote_char);
	quoted_str = ft_strndup(start, (end - start) + 1);
	token = create_token(T_QUOTE, quoted_str);
	add_token(token_list, token);
	free(quoted_str);
	*p = end + 1;
}
