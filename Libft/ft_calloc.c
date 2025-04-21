/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 15:25:46 by isalayan          #+#    #+#             */
/*   Updated: 2024/06/18 16:03:38 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*s;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb > 2147483647 / size)
		return (NULL);
	s = (void *)malloc(nmemb * size);
	if (!s)
		return (NULL);
	ft_bzero(s, nmemb * size);
	return (s);
}
