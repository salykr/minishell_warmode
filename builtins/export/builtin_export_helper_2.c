/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:57:01 by marvin            #+#    #+#             */
/*   Updated: 2025/01/02 16:39:00 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

static size_t	count_backslashes(const char *str, size_t *index)
{
	size_t	count;

	count = 0;
	while (str[*index] == '\\')
	{
		count++;
		(*index)++;
	}
	return (count);
}

static void	handle_backslashes(char *total, size_t *j,
			size_t backslash_count, char next_char)
{
	if (next_char == '$')
	{
		while (backslash_count--)
			total[(*j)++] = '\\';
	}
	else if (backslash_count % 2 == 0)
	{
		while (backslash_count--)
			total[(*j)++] = '\\';
	}
	else
	{
		while (--backslash_count)
			total[(*j)++] = '\\';
	}
}

int	initialise_escape_char(size_t *i, size_t *j, char *str, char **total)
{
	*i = 0;
	*j = 0;
	*total = malloc(ft_strlen(str) + 1);
	if (!total)
		return (1);
	return (0);
}

char	*ft_escape_char(char *str)
{
	char	*total;
	size_t	i;
	size_t	j;
	size_t	backslash_count;
	char	next_char;

	if (initialise_escape_char(&i, &j, str, &total) == 1)
		return (NULL);
	while (str[i] != '\0')
	{
		if (str[i] == '\\')
		{
			backslash_count = count_backslashes(str, &i);
			if (str[i])
				next_char = str[i];
			else
				next_char = '\0';
			handle_backslashes(total, &j, backslash_count, next_char);
		}
		else
			total[j++] = str[i++];
	}
	total[j] = '\0';
	return (free(str), total);
}

char	*ft_trim_string(char *str)
{
	char	*trimmed_str;

	trimmed_str = ft_strtrim(str, " ");
	if (trimmed_str)
	{
		free(str);
		return (trimmed_str);
	}
	return (NULL);
}
