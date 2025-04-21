/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:59:55 by isalayan          #+#    #+#             */
/*   Updated: 2024/06/21 09:41:42 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>  // For read function
# include <stdlib.h>  // For malloc, free
# include <fcntl.h>   // For open function
# include <stdio.h>   // For printf function (used in main)
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*read_line(int fd, char *buffer, int *buf_pos, int *buf_size);
ssize_t	buffer_read(int fd, char *buffer, int *buf_pos, int *buf_size);
char	*append_char_to_line(char *line, char c, int len);
#endif 
