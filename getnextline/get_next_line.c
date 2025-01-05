/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:54:32 by skreik            #+#    #+#             */
/*   Updated: 2024/06/27 17:52:05 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
//int buffer_reset_count = 0;

char	*get_line_left(char *str)
{
	int		i;
	char	*line;

	i = 0;
	while (str[i] != '\n' && str[i])
		i++;
	if (str[i] && str[i] == '\n')
		i++;
	line = (char *)malloc(i + 1);
	if (!line)
		return (NULL);
	line[i] = '\0';
	while (i-- > 0)
		line[i] = str[i];
	return (line);
}

char	*get_line_right(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*line;

	len = 0;
	while (str[len])
		len++;
	i = 0;
	while (str[i] != '\n' && str[i])
		i++;
	if (str[i] && str[i] == '\n')
		i++;
	line = (char *)malloc((len - i) + 1);
	if (!line)
		return (NULL);
	j = 0;
	while (str[i])
		line[j++] = str[i++];
	line[j] = '\0';
	return (line);
}

char	*extract_from_str(char **storage)
{
	char	*line;
	char	*temp;

	if (*storage == NULL || **storage == '\0')
		return (NULL);
	temp = ft_strdup(*storage);
	free(*storage);
	line = get_line_left(temp);
	*storage = get_line_right(temp);
	free(temp);
	return (line);
}

char	*read_from_file(int fd, char **storage)
{
	char	*buff;
	char	*temp;
	int		rvalue;

	rvalue = -1;
	buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	while (!char_check(*storage))
	{
		rvalue = read(fd, buff, BUFFER_SIZE);
		if (rvalue <= 0)
			break ;
		buff[rvalue] = '\0';
		temp = ft_strjoin(*storage, buff);
		free(*storage);
		*storage = temp;
	}
	free(buff);
	if (rvalue == 0 && **storage != '\0')
		return (extract_from_str(storage));
	if (rvalue > 0 || char_check(*storage))
		return (extract_from_str(storage));
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*storage = NULL;
	char		*line;

	if (!storage)
		storage = ft_strdup("");
	line = read_from_file(fd, &storage);
	if (!line && storage && *storage == '\0')
	{
		free(storage);
		storage = NULL;
	}
	return (line);
}
