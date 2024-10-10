/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:35:45 by skreik            #+#    #+#             */
/*   Updated: 2024/09/06 14:42:31 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*next_token = NULL;
    char		*start;
    char		*end;

	if (str != NULL)
        next_token = str;
	if (next_token == NULL)
        return (NULL);
	start = next_token;
	while (*start && ft_strchr(delim, *start))
        start++;
    if (*start == '\0')
    {
        next_token = NULL;
		return (NULL);
    }
    end = start;
    while (*end && !ft_strchr(delim, *end))
        end++;
    if (*end)
    {
        *end = '\0';
		next_token = end + 1;
    }
	else
		next_token = NULL;
    return (start);
}
