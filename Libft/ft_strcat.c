/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:56:51 by skreik            #+#    #+#             */
/*   Updated: 2024/09/05 12:59:12 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strcat(char *dest, const char *src) 
{
    char *dst;

    dst= dest;
    // Move the pointer to the end of the destination string
    while (*dst)
        dst++;
    // Copy the source string to the end of the destination string
    while (*src)
        *dst++ = *src++;
    *dst = '\0';  // Null-terminate the concatenated string
    return (dest);
}