/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:51:08 by marvin            #+#    #+#             */
/*   Updated: 2024/12/29 17:51:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int get_last_input_redirection(int *redirection)
{
    int i;
    int last_index;

	i = 0;
	last_index = -1;
    while (redirection && redirection[i] != '\0')
    {
        if (redirection[i] == T_HEREDOC || redirection[i] == T_INPUT)
            last_index = i;
        i++;
    }
    return (last_index);
}
int handle_heredoc_redirection(t_parser *parser, int *fd_heredoc)
{
    if (parser->redirection && check_heredoc_existence(parser->redirection))
    {
        write_in_heredoc(parser);
        if (global_var == 130)
            return -1;
        *fd_heredoc = handle_heredoc(parser->heredoc);
    }
    return 0;
}

int process_input_redirection(t_parser *parser, int *fd_file)
{
    int i;
    int j;
	
	i = -1;
	j = -1;
    while (parser->redirection && parser->redirection[++i] != '\0')
    {
        if (parser->redirection[i] == T_INPUT)
        {
            if (parser->infile != NULL && parser->infile[++j] != NULL)
            {
                if (!check_permissions(parser->infile[j], 0))
                    return (global_var = 1, 1);
                if (j != 0)
                    close(*fd_file);
                *fd_file = open(parser->infile[j], O_RDONLY);
                if (*fd_file == -1)
                    perror("Error: Input file");
            }
        }
    }
    return (0);
}

int set_final_fd(t_parser *parser, int fd_file, int fd_heredoc, int *fd)
{
    if (parser->redirection[get_last_input_redirection(parser->redirection)] == T_HEREDOC)
    {
        if (fd_file != -1)
            close(fd_file);
        *fd = fd_heredoc;
    }
    else if (parser->redirection[get_last_input_redirection(parser->redirection)] == T_INPUT)
    {
        if (fd_heredoc != -1)
            close(fd_heredoc);
        *fd = fd_file;
    }
    return (0);
}

int manage_redirection_input(t_parser *parser, int *fd)
{
    int fd_heredoc = -1;
    int fd_file = -1;

    if (handle_heredoc_redirection(parser, &fd_heredoc) == -1)
        return (-1);
    if (process_input_redirection(parser, &fd_file) != 0)
        return (1);
    return (set_final_fd(parser, fd_file, fd_heredoc, fd));
}



// int manage_redirection_input(t_parser *parser, int *fd)
// {
//     int i;
//     int j;
//     int fd_heredoc;
//     int fd_file;

//     fd_file = -1;
//     fd_heredoc = -1;
//     i = -1;
//     j = -1;
//     if (parser->redirection && check_heredoc_existence(parser->redirection))
//     {
//         write_in_heredoc(parser);
// 		if(global_var == 130)
// 			return (-1);
//         fd_heredoc = handle_heredoc(parser->heredoc);
//     }
//     while (parser->redirection && parser->redirection[++i] != '\0')  // Pre-increment i
//     {
//         if (parser->redirection[i] == T_INPUT)
//         {
//             if (parser->infile != NULL && parser->infile[++j] != NULL)  // Pre-increment j
//             {
// 				if (!check_permissions(parser->infile[j], 0))
//                     return(global_var = 1, 1);
//                 if (j != 0)
//                     close(fd_file);
//                 fd_file = open(parser->infile[j], O_RDONLY);
//                 if (fd_file == -1)
//                     perror("Error: Input file");
//             }
//         }
//     }
//     if (parser->redirection[get_last_input_redirection(parser->redirection)] == T_HEREDOC)
//     {
//         if (fd_file != -1)
//             close(fd_file);
//         *fd = fd_heredoc;
//     }
//     else if (parser->redirection[get_last_input_redirection(parser->redirection)] == T_INPUT)
//     {
//         if (fd_heredoc != -1)
//             close(fd_heredoc);
//         *fd = fd_file;
//     }
// 	return(0);
// }
