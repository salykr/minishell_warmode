/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:55:18 by skreik            #+#    #+#             */
/*   Updated: 2024/09/05 15:44:57 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	char	*dst;

	dst = dest;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (dest);
}
