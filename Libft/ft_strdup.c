/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:46:24 by skreik            #+#    #+#             */
/*   Updated: 2024/06/13 11:46:25 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

char	*ft_strdup(const char *s)
{
	unsigned int	i;
	unsigned int	size;
	char			*dest;

	i = 0;
	size = ft_strlen(s) + 1;
	if (s == NULL)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * size);
	if (!(dest))
		return (NULL);
	else
	{
		while (s[i] != '\0')
		{
			dest[i] = s[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (dest);
}
