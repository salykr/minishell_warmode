/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:15:47 by skreik            #+#    #+#             */
/*   Updated: 2024/06/14 10:15:49 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_digitcount(int n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
		count++;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static int	ft_sign(int n)
{
	if (n < 0)
		return (-1);
	return (1);
}

static int	absolute(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

char	*ft_itoa(int n)
{
	int		digit_num;
	int		sign;
	char	*num;

	digit_num = ft_digitcount(n);
	sign = ft_sign(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	num = (char *)malloc(sizeof(char) * (digit_num + 1));
	if (!num)
		return (NULL);
	num[digit_num--] = '\0';
	while (digit_num >= 0)
	{
		if (sign < 0 && digit_num == 0)
			num[0] = '-';
		else
		{
			num[digit_num] = absolute(n) % 10 + '0';
			n /= 10;
		}
		digit_num--;
	}
	return (num);
}
