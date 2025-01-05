/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_7.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:40:13 by marvin            #+#    #+#             */
/*   Updated: 2024/11/18 16:40:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mini_shell.h"

int	handle_shlvl_case(char *name, char *value, t_name_value *new_nv)
{
	if (ft_strcmp("SHLVL", name) == 0)
	{
		new_nv->new_name = name;
		new_nv->new_value = value;
		return (1);
	}
	return (2);
}

int	has_equals_inside_quotes(char *str, int start, char quote_type)
{
	int	i;
	int	has_equals;

	has_equals = 0;
	i = start;
	while (str[i] != '\0' && str[i] != quote_type)
	{
		if (str[i] == '=')
			has_equals = 1;
		i++;
	}
	return (has_equals);
}

void	copy_until_quote(char *str, char *result, int *i, int *j)
{
	ft_strncat(result, str + *j, *i - *j);
	*j = *i;
}

void	fix_quote(char *str, char *result, int *i, int *j)
{
	char		quote_type;
	int			quote_start;
	int			has_equals;
	int			quote_end;

	quote_type = str[*i];
	quote_start = *i;
	(*i)++;
	has_equals = has_equals_inside_quotes(str, *i, quote_type);
	quote_end = *i;
	while (str[quote_end] != '\0' && str[quote_end] != quote_type)
		quote_end++;
	if (has_equals)
		ft_strncat(result, str + *i, quote_end - *i);
	else
		ft_strncat(result, str + quote_start, quote_end - quote_start + 1);
	*i = quote_end + 1;
	*j = *i;
}

char	*remove_closing_quote_after_equals(char *str)
{
	int		len;
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '"' || str[i] == '\'')
			fix_quote(str, result, &i, &j);
		else
		{
			copy_until_quote(str, result, &i, &j);
			i++;
		}
	}
	return (free(str), result);
}
