/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saly <saly@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:55:39 by saly              #+#    #+#             */
/*   Updated: 2024/12/31 10:57:12 by saly             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long ft_strtoll(const char *nptr, char **endptr, int base)
{
	return(strtoll(nptr, endptr, base));
}