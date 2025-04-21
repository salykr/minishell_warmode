/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:01:14 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 13:10:32 by isalayan         ###   ########.fr       */
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
