/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:03:06 by marvin            #+#    #+#             */
/*   Updated: 2025/01/12 14:39:45 by skreik           ###   ########.fr       */
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
		new_string = ft_itoa(g_v);
	else if ((is_num_or_char(str[1]) == 0 && (str[1] != '_'))
		&& str[0] == '$' && (inside_quote == 0 || inside_quote == 1))
		new_string = ft_strdup(str);
	else if ((is_num_or_char(str[1]) == 1 || (str[1] == '_')) && str[0] == '$'
		&& (inside_quote == 0 || inside_quote == 1))
		new_string = get_env_value(envp, str + 1);
	return (new_string);
}

int	is_num_or_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

/*
char *create_array_till_dollar(char *str, int index);
int find_dollar_pos1(char *str);
int find_end_variable(char *str,int j);
void check_quotes_status_and_update(int *inside_quote, int *d_start,int *d_end, int *s_start,int *s_end , char c);
int is_num_or_char(char c);
void check_quotes_till_end(char *str,int *inside_quote, int *d_start,int *d_end, int *s_start,int *s_end,int start,int end);
char *check_char_after_dollar(char *str, int inside_quote,t_env *envp);
char *create_array_till_dollar(char *str, int index);
void expand_and_replace(char **str, char *prefix, int end);

char *process_variable(char *x_str, t_env *env)
{
    int i = 0;
    int end = 0;
    char *to_expand = NULL;
    int start = 0;
    char *expanded = NULL;
    int inside_quote = 0; // 1 for "" and 2 for ' and 0 outside
    int d_start = 0, d_end = 0, s_start = 0, s_end = 0;
    char *temp;
    char *prefix;
    char *str;
    if (ft_strchr(x_str, '$') == NULL)
        return (remove_paired_quotes(ft_strdup(x_str)));
    str = strdup(x_str);
    while (str[i])
    {
        check_quotes_status_and_update(&inside_quote, &d_start, &d_end, &s_start, &s_end, str[i]);
        if (str[i] == '$')
        {
            prefix = create_array_till_dollar(str, i);
            start = i;
            end = find_end_variable(str, i);
            to_expand = strndup(str + start, end - start);
            expanded = check_char_after_dollar(to_expand, inside_quote, env);
            free(to_expand);
            if (expanded == NULL && inside_quote != 2)
            {
                expand_and_replace(&str, prefix, end);
                i = strlen(prefix) - 1;
                free(prefix);
            }
            else if (expanded)
            {
                temp = ft_strjoin(prefix, expanded);
                free(prefix);
                prefix = temp;                
                expand_and_replace(&str, prefix, end);
                i = strlen(prefix) - 1;
                free(temp);
            }
            else
            {
                i = end;
                free(prefix);
            }
            check_quotes_till_end(str, &inside_quote, &d_start, &d_end, &s_start, &s_end, start, end);
        }
        i++;
    }
    return (remove_paired_quotes(str));
}

void expand_and_replace(char **str, char *prefix, int end)
{
    if (!prefix[0])
    {
        free(*str);
        *str = strdup("");
        return;
    }
    char *suffix = strdup(*str + end);
    int len = strlen(prefix) + strlen(suffix);
    char *new_str = malloc(len + 1);
    int i = 0, x = 0;
    while (prefix[i])
        new_str[x++] = prefix[i++];
    i = 0;
    while (suffix[i])
        new_str[x++] = suffix[i++];
    new_str[x] = '\0';
    free(*str);
    free(suffix);
    *str = new_str;
}

void check_quotes_till_end(char *str, int *inside_quote, int *d_start, int *d_end, int *s_start, int *s_end, int start, int end) {
    int i;

    i = start;
    while (i <= end && str[i] != '\0' && i < (int)ft_strlen(str)) {
        check_quotes_status_and_update(inside_quote, d_start, d_end, s_start, s_end, str[i]);
        i++;
    }
}


char *create_array_till_dollar(char *str, int index)
{
    char *new_string = malloc(index + 1);
    for (int i = 0; i < index; i++)
        new_string[i] = str[i];
    new_string[index] = '\0';
    return new_string;
}

int find_dollar_pos11(char *str, int j)
{
    int i = j;
    while(str[i])
    {
        if(str[i] == '$')
            return(i);
        i++;
    }
    return(-42);
}

int find_end_variable(char *str, int j)
{ 
    int i = find_dollar_pos11(str,j);
    i++;
    if (str[i] == '\"' || str[i] == '\'')
        return(i + 1);
    if (str[i] == '?')
        return(i + 1);
    while (str[i] && ((str[i] >= 'a' && str[i] <= 'z')
            || (str[i] >= 'A' && str[i] <= 'Z')
            || (str[i] >= '0' && str[i] <= '9')
            || str[i] == '_' ))
    {
        i++;
    }
    return(i);
}

int find_dollar_pos1(char *str)
{
    int i = 0;
    while(str[i])
    {
        if(str[i] == '$')
            return(i);
        i++;
    }
    return(-42);
}

void   check_quotes_status_and_update(int *inside_quote, int *d_start,int *d_end, int *s_start,int *s_end , char c)
{
    if ((c == '"' || c == '\'') && (*inside_quote) == 0)
    {
        if(c == '"')
        {
            (*inside_quote) = 1;
            (*d_start) = 1;
            *d_end = 0;
        }
        if(c == '\'')
        {
            (*inside_quote) = 2;
            (*s_start) = 1;
            *s_end = 0;
        }
    }
    else if  ((c == '"' || c == '\'') && (*inside_quote) != 0)
    {
        if(c == '"'&& *s_start != 1)
        {
            (*inside_quote) = 0;
            (*d_end) = 1;
        }
        if(c == '\'' && *d_start != 1)
        {
            (*inside_quote) = 0;
            (*s_end) = 1;
        }
    }
    if (*s_end == 1 && *s_start == 1)
    {
        (*s_start) = 0;
        *s_end = 0;
        (*inside_quote) = 0;
    }
    if (*d_end == 1 && *d_start == 1)
    {
        (*d_start) = 0;
        *d_end = 0;
        (*inside_quote) = 0;
    }
}

char *expand_dollar(char *str,t_env *envp)
{
    char *to_replace;
    to_replace = ft_getenv(envp,str + 1);
    return(to_replace);
}

static int ft_sign(int n)
{
    if (n < 0)
        return (-1);
    else 
        return (1);
}
int ft_absolute(int n)
{
    if (n < 0) 
        return (-n);
    else
        return (n);
}
char *ft_itoa_static(int n)
{
    static char num[12];
    int digit_num;
    int abs_n;
    int sign;

    digit_num = 11;
    sign = ft_sign(n);
    num[digit_num--] = '\0'; // Null-terminate the string
    if (n == -2147483648)
        return ("-2147483648");
    if (n == 0) 
    {
        num[digit_num] = '0';
        return (&num[digit_num]);
    }
    abs_n = ft_absolute(n);
    while (abs_n > 0)
    {
        num[digit_num--] = (abs_n % 10) + '0';
        abs_n /= 10;
    }
    if (sign < 0)
        num[digit_num--] = '-';
    return (&num[digit_num + 1]);
}


char *check_char_after_dollar(char *str, int inside_quote, t_env *envp)
{
    char *new_string;

    new_string = NULL;
    if (str[0] == '$' && (str[1] == '"' || str[1] == '\'') && (inside_quote == 0))
        new_string = strdup(str + 1);
    else if (str[0] == '$' && (str[1] >= '0' && str[1] <= '9') && (inside_quote == 0 || inside_quote == 1))
        new_string = strdup(str + 2);
    else if (str[0] == '$' && str[1] == '?')
        new_string = ft_itoa_static(g_v);
    else if ((is_num_or_char(str[1]) == 0 && (str[1] != '_')) && str[0] == '$' && (inside_quote == 0 || inside_quote == 1))
        new_string = strdup(str);
    else if ((is_num_or_char(str[1]) == 1 || (str[1] == '_')) && str[0] == '$' && (inside_quote == 0 || inside_quote == 1))
        new_string = ft_getenv(envp, str + 1);
    return (new_string);
}

int is_num_or_char(char c)
{
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    {
        return(1);
    }
    return (0);
}
*/
