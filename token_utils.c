/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 22:48:41 by marvin            #+#    #+#             */
/*   Updated: 2024/11/11 22:48:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./mini_shell.h"

void	ft_find_str_helper(const char ***str, const char **last_c, char c)
{
	while (***str)
	{
		if (***str == c)
		{
			while (***str && ***str != ' ')
				(**str)++;
			*last_c = **str - 1;
			break ;
		}
		(**str)++;
	}
}

char	*ft_find_str(const char **str, char c)
{
	const char *last_c = NULL;
	ft_find_str_helper(&str, &last_c, c);
	if (last_c && *(last_c + 1) == '=')
	{
		last_c++;
		while (*last_c && *last_c != ' ')
			last_c++;
		while (*last_c != c && *last_c && *last_c != ' ')
			last_c++;
	}
	if (!last_c)
	{
		last_c = *str - 1;
		while (last_c >= *str && ft_isspace(*last_c))
			last_c--;
	}
	// while (*last_c && ft_isspace(*last_c))
	// 	last_c++;
	// while (*last_c && ft_isascii(*last_c))
	// 	last_c++;
	return ((char *)last_c);
}
