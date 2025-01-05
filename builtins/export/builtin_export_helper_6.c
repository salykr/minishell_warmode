/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_6.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:51:08 by marvin            #+#    #+#             */
/*   Updated: 2024/11/16 19:51:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	has_quotes(char *str)
{
	return (ft_strchr(str, '\'') || ft_strchr(str, '\"'));
}

void	process_quotes(char *str, char *result, t_ij *indexes, char quote_type)
{
	int	start;

	start = indexes->i + 1;
	indexes->i += 1;
	ft_strncat(result, str + indexes->j, indexes->i - indexes->j - 1);
	while (str[indexes->i] && str[indexes->i] != quote_type)
		indexes->i++;
	if (str[indexes->i] == quote_type)
	{
		ft_strncat(result, str + start, indexes->i - start);
		indexes->i++;
	}
	indexes->j = indexes->i;
}

char	*remove_paired_quotes(char *str)
{
	int		len;
	char	*result;
	t_ij	indexes;

	indexes.i = 0;
	indexes.j = 0;
	if (!str || !has_quotes(str))
		return (str);
	len = ft_strlen(str);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (str[indexes.i])
	{
		if (str[indexes.i] == '"' || str[indexes.i] == '\'')
			process_quotes(str, result, &indexes, str[indexes.i]);
		else
			indexes.i++;
	}
	if (indexes.j < len)
		ft_strncat(result, str + indexes.j, len - indexes.j);
	return (free(str), result);
}
