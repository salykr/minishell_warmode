/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:55:39 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 13:04:23 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_strtoll(const char *nptr, char **endptr, int base)
{
	return (strtoll(nptr, endptr, base));
}
