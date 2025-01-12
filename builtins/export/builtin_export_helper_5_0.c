/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:18:23 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 19:18:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*add_path(char *str)
{
	char	*new_path;

	if (str[0] == '.')
	{
		if (str[1] == '.')
		{
			new_path = ft_str_join("$OLDPWD", str + 2);
			free(str);
			return (new_path);
		}
		new_path = ft_str_join("$PWD", str + 1);
		free(str);
		return (new_path);
	}
	return (str);
}

char	*check_tilde(char *str)
{
	char	*new_str;

	if (str[0] == '~')
	{
		new_str = ft_str_join("$HOME", str + 1);
		free(str);
		return (new_str);
	}
	return (str);
}

void	expand_and_replace(char **str, char *prefix, int end)
{
	char	*suffix;
	char	*new_str;
	int		len;
	int		i;
	int		x;

	if (!prefix[0])
	{
		free(*str);
		*str = ft_strdup("");
		return ;
	}
	suffix = ft_strdup(*str + end);
	len = ft_strlen(prefix) + ft_strlen(suffix);
	new_str = malloc(len + 1);
	i = 0;
	x = 0;
	while (prefix[i])
		new_str[x++] = prefix[i++];
	i = 0;
	while (suffix[i])
		new_str[x++] = suffix[i++];
	new_str[x] = '\0';
	handle_memory_errors(*str, suffix);
	*str = new_str;
}

void	check_quotes_till_end(char *str, t_quoted *q, int start, int end)
{
	int	i;

	i = start;
	while (i <= end && str[i] != '\0' && i < (int)ft_strlen(str))
	{
		check_quotes_status_and_update(q, str[i]);
		i++;
	}
}
