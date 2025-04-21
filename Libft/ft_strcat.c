/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:56:51 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 13:06:33 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	char	*dst;

	dst = dest;
	while (*dst)
		dst++;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (dest);
}
