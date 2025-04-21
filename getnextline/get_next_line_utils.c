/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:56:52 by isalayan          #+#    #+#             */
/*   Updated: 2024/06/20 16:47:23 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*append_char_to_line(char *line, char c, int len)
{
	char	*new_line;
	int		i;

	new_line = (char *)malloc(len + 2);
	i = 0;
	if (!new_line)
		return (NULL);
	while (i < len)
	{
		new_line[i] = line[i];
		i++;
	}
	new_line[len] = c;
	new_line[len + 1] = '\0';
	free(line);
	return (new_line);
}

ssize_t	buffer_read(int fd, char *buffer, int *buf_pos, int *buf_size)
{
	*buf_size = read(fd, buffer, BUFFER_SIZE);
	*buf_pos = 0;
	return (*buf_size);
}

char	*read_line(int fd, char *buffer, int *buf_pos, int *buf_size)
{
	char	*line ;
	int		line_len;
	char	c;

	line = NULL;
	line_len = 0;
	while (1)
	{
		if (*buf_pos >= *buf_size)
		{
			if (buffer_read(fd, buffer, buf_pos, buf_size) <= 0)
			{
				if (line_len > 0)
					return (line);
				free(line);
				return (NULL);
			}
		}
		c = buffer[(*buf_pos)++];
		line = append_char_to_line(line, c, line_len++);
		if (!line || c == '\n')
			break ;
	}
	return (line);
}
