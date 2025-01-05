/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:01:14 by skreik            #+#    #+#             */
/*   Updated: 2024/09/05 15:13:16 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *start, size_t length)
{
	char	*dup;
	char	*dst;

	dup = (char *)malloc(length + 1);
	if (dup == NULL)
		return (NULL);
	dst = dup;
	while (length-- && *start)
		*dst++ = *start++;
	*dst = '\0';
	return (dup);
}
