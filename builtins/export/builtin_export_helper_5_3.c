/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5_3.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 14:45:20 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/19 11:59:13 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	initialize_process_variable(t_quoted *q)
{
	q->d_start = 0;
	q->d_end = 0;
	q->s_start = 0;
	q->s_end = 0;
	q->inside_quote = 0;
}

char	*process_variable(char *x_str, t_env *env)
{
	int			i;
	t_quoted	q;
	char		*str;

	i = 0;
	initialize_process_variable(&q);
	if (ft_strchr(x_str, '$') == NULL && x_str[0] != '~')
		return (remove_paired_quotes(ft_strdup(x_str)));
	str = ft_strdup(x_str);
	str = check_tilde(str);
	while (str[i])
	{
		check_quotes_status_and_update(&q, str[i]);
		if (str[i] == '$')
			process_variable_helper(&i, &str, &q, env);
		i++;
	}
	return (remove_paired_quotes(str));
}

void	process_variable_helper(int *i, char **str, t_quoted *q, t_env *env)
{
	int			start;
	char		*to_expand;
	t_strings	s;

	if (*i == 0)
	{
		start = 0;
		s.end = 0;
		s.expanded = NULL;
		to_expand = NULL;
	}
	s.prefix = create_array_till_dollar((*str), (*i));
	start = (*i);
	s.end = find_end_variable((*str), (*i));
	to_expand = ft_strndup((*str) + start, s.end - start);
	s.expanded = check_char_after_dollar(to_expand, q->inside_quote, env);
	free(to_expand);
	process_variable_helper_elseif(i, &s, str, q);
	check_quotes_till_end((*str), q, start, s.end);
}

void	process_variable_helper_elseif(int *i, t_strings *s,
				char **str, t_quoted *q)
{
	char	*temp;

	if (s->expanded == NULL && q->inside_quote != 2)
	{
		expand_and_replace(str, s->prefix, s->end);
		(*i) = ft_strlen(s->prefix) - 1;
		free(s->prefix);
	}
	else if (s->expanded)
	{
		temp = ft_strjoin(s->prefix, s->expanded);
		free(s->prefix);
		s->prefix = temp;
		expand_and_replace(str, s->prefix, s->end);
		(*i) = ft_strlen(s->prefix) - 1;
		free(temp);
		free(s->expanded);
	}
	else
	{
		(*i) = s->end;
		free(s->prefix);
	}
}

/*
   char *process_variable(char *x_str, t_env *env)
   {
   int i;
   int end;
   char *to_expand;
   int start;
   char *expanded;
   t_quoted q;
   char *temp;
   char *prefix;
   char *str;

   i = 0;
   end = 0;
   initialize_process_variable(&q);
   to_expand = NULL;
   expanded = NULL;
   start = 0;
   if (ft_strchr(x_str, '$') == NULL && x_str[0] != '~')
   return (remove_paired_quotes(ft_strdup(x_str)));
   str = ft_strdup(x_str);
   str = check_tilde(str);
   while (str[i])
   {
   check_quotes_status_and_update(&q, str[i]);
   if (str[i] == '$')
   {
   prefix = create_array_till_dollar(str, i);
   start = i;
   end = find_end_variable(str, i);
   to_expand = ft_strndup(str + start, end - start);
   expanded = check_char_after_dollar(to_expand, q.inside_quote, env);
   free(to_expand);
   if (expanded == NULL && q.inside_quote != 2)
   {
   expand_and_replace(&str, prefix, end);
   i = ft_strlen(prefix) - 1;
   free(prefix);
   }
   else if (expanded)
   {
   temp = ft_strjoin(prefix, expanded);
   free(prefix);
   prefix = temp;
   expand_and_replace(&str, prefix, end);
   i = ft_strlen(prefix) - 1;
   free(temp);
   }
   else
   {
   i = end;
   free(prefix);
   }
   check_quotes_till_end(str, &q, start, end);
   }
   i++;
   }
   return (remove_paired_quotes(str));
   }

*/
// export hi$?=hi'$USER'''$USER''"$USER"$?$HOME$9.
