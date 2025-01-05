/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:49:29 by skreik            #+#    #+#             */
/*   Updated: 2024/06/11 13:49:31 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*cdest;
	const char	*csrc;
	size_t		i;

	i = 0;
	cdest = (char *)dest;
	csrc = (const char *)src;
	if (dest == src || (!cdest && !src))
		return (dest);
	if (src > dest)
	{
		while (i < n)
		{
			cdest[i] = csrc[i];
			i++;
		}
	}
	else if (src < dest)
	{
		i = n ;
		while (i-- > 0)
			cdest[i] = csrc[i];
	}
	return (dest);
}
