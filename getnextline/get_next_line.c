/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:02:27 by isalayan          #+#    #+#             */
/*   Updated: 2024/08/06 14:51:23 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	static int	buf_pos = 0;
	static int	buf_size = 0;

	if (BUFFER_SIZE < 1 || fd < 0)
		return (NULL);
	return (read_line(fd, buffer, &buf_pos, &buf_size));
}
