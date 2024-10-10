/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:19:25 by skreik            #+#    #+#             */
/*   Updated: 2024/06/12 14:19:29 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*sc1 = (const unsigned char *)s1;
	const unsigned char	*sc2 = (const unsigned char *)s2;

	while (n--)
	{
		if (*sc1 != *sc2)
			return (*sc1 - *sc2);
		sc1++;
		sc2++;
	}
	return (0);
}
