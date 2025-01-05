/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 14:18:29 by skreik            #+#    #+#             */
/*   Updated: 2024/06/28 14:18:31 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>

size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
int		char_check(const char *s);
char	*get_line_left(char *str);
char	*get_line_right(char *str);
char	*extract_from_str(char **storage);
char	*read_from_file(int fd, char **storage);
char	*get_next_line(int fd);

#endif
