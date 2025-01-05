/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:37:16 by marvin            #+#    #+#             */
/*   Updated: 2024/11/24 22:37:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"

int	is_builtin(t_parser *parser)
{
	if (parser->command == NULL)
		return (0);
	if (strcmp(parser->command, "cd") == 0
		|| strcmp(parser->command, "pwd") == 0 || strcmp(parser->command, "export") == 0
		|| strcmp(parser->command, "unset") == 0 || strcmp(parser->command, "env") == 0
		|| strcmp(parser->command, "exit") == 0 ||strcmp(parser->command, "echo") == 0)
	{
		return (1);
	}
	return (0);
}

void manage_input_output( t_fd *f, int fd[2], t_parser *parser)
{
	// (void)fd;
	// printf("in manage input output fd_1: %d fd_ 2:%d\n",f->fd_1,f->fd_2);
	// printf("in manage input output fd[0]: %d fd[1]:%d\n",fd[0],fd[1]);
	if (f->fd_1 != STDIN_FILENO)
	{
		dup2(f->fd_1, STDIN_FILENO); // Redirect input
		close(f->fd_1);
		// if(fd[1] != -1)
			// close(fd[1]);
	}
	if (f->fd_2 != STDOUT_FILENO)
	{
		dup2(f->fd_2, STDOUT_FILENO); // Redirect output
		close(f->fd_2);
		// if(fd[0] != -1)
			// close(fd[0]);
	}
	if(parser->prev != NULL)
		close(fd[0]);
	if(parser->next != NULL)
		close(fd[1]);
	
}

void initialize_heredoc(int *heredoc_fd, t_parser *parser)
{
	*heredoc_fd = -1;
	if (parser->delimeter != NULL && parser->redirection != NULL)
	{
		*heredoc_fd = handle_heredoc(parser->heredoc);
	}
}
void initialize_execution(t_parser *parser,t_env *env,char **cmd_path)
{
	if (!strncmp(parser->command, "./", 2))
		*cmd_path = get_path_PWD(*env, parser->command);
	else
		*cmd_path = get_path(*env, parser->command);
}
void handle_child_exit(pid_t pid,t_fd *f, t_parser *parser)
{

	(void)pid;
	(void)parser;

    if (f->fd_2 != STDOUT_FILENO)
		close(f->fd_2);

	if (f->fd_1!= STDIN_FILENO)
        close(f->fd_1);
	
}
