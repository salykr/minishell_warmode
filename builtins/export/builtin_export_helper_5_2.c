/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:03:06 by marvin            #+#    #+#             */
/*   Updated: 2025/01/12 14:39:45 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	check_quote_opening(t_quoted *q, char c)
{
	if (c == '"')
	{
		q->inside_quote = 1;
		q->d_start = 1;
		q->d_end = 0;
	}
	if (c == '\'')
	{
		q->inside_quote = 2;
		q->s_start = 1;
		q->s_end = 0;
	}
}

void	check_quote_closing(t_quoted *q, char c)
{
	if (c == '"' && q->s_start != 1)
	{
		q->inside_quote = 0;
		q->d_end = 1;
	}
	if (c == '\'' && q->d_start != 1)
	{
		q->inside_quote = 0;
		q->s_end = 1;
	}
}

void	reset_quote_flags(t_quoted *q)
{
	if (q->s_end == 1 && q->s_start == 1)
	{
		q->s_start = 0;
		q->s_end = 0;
		q->inside_quote = 0;
	}
	if (q->d_end == 1 && q->d_start == 1)
	{
		q->d_start = 0;
		q->d_end = 0;
		q->inside_quote = 0;
	}
}

void	check_quotes_status_and_update(t_quoted *q, char c)
{
	if ((c == '"' || c == '\'') && q->inside_quote == 0)
		check_quote_opening(q, c);
	else if ((c == '"' || c == '\'') && q->inside_quote != 0)
		check_quote_closing(q, c);
	reset_quote_flags(q);
}

char	*check_char_after_dollar(char *str, int inside_quote, t_env *envp)
{
	char	*new_string;

	new_string = NULL;
	if (str[0] == '$' && (str[1] == '"' || str[1] == '\'')
		&& (inside_quote == 0))
		new_string = ft_strdup(str + 1);
	else if (str[0] == '$' && (str[1] >= '0' && str[1] <= '9')
		&& (inside_quote == 0 || inside_quote == 1))
		new_string = ft_strdup(str + 2);
	else if (str[0] == '$' && str[1] == '?')
		new_string = ft_itoa(envp->exit_code);
	else if ((is_num_or_char(str[1]) == 0 && (str[1] != '_'))
		&& str[0] == '$' && (inside_quote == 0 || inside_quote == 1))
		new_string = ft_strdup(str);
	else if ((is_num_or_char(str[1]) == 1 || (str[1] == '_')) && str[0] == '$'
		&& (inside_quote == 0 || inside_quote == 1))
		new_string = get_env_value(envp, str + 1);
	return (new_string);
}
