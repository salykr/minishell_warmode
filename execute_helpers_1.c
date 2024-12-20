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
void manage_input_output(int heredoc_fd, t_fd *f, int fd[2],t_parser *parser)
{
	(void)parser;
	if (heredoc_fd != -1)
	{
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	else if (f->fd_1 != STDIN_FILENO)
	{
		dup2(f->fd_1, STDIN_FILENO); // Redirect input
		//fd[1] != -1 && 
		if(f->fd_1 == fd[0])
			close(fd[1]);
		close(f->fd_1);
	}
	if (f->fd_2 != STDOUT_FILENO)
	{
		dup2(f->fd_2, STDOUT_FILENO); // Redirect output
		//fd[0] != -1 && 
		if(f->fd_2 == fd[1])
			close(fd[0]);
		close(f->fd_2);
	}
}
void initialize_heredoc(int *heredoc_fd, t_parser *parser)
{
	*heredoc_fd = -1;
	if (parser->delimeter != NULL && parser->redirection != NULL)
	{
		if (parser->redirection[0] == T_HEREDOC)
			*heredoc_fd = handle_heredoc(parser);
	}
}
char ** initialize_execution(int *heredoc_fd, t_parser *parser,t_env *env,char **cmd_path)
{
	initialize_heredoc(heredoc_fd, parser);
	printf("in initialize_execution\n");
	print_2d_array(parser->heredoc);
	if (!strncmp(parser->command, "./", 2))
		*cmd_path = get_path_PWD(*env, parser->command);
	else
		*cmd_path = get_path(*env, parser->command);
	return(ft_create_args(parser));
}
void handle_child_exit(pid_t pid, int *heredoc_fd, t_fd *f, t_parser *parser)
{
	(void)pid;
    // int status;

    // waitpid(pid, &status, 0); // Wait for child process to finish
    // if (WIFEXITED(status)) 
    //     global_var = WEXITSTATUS(status);
	if (f->fd_1 != STDIN_FILENO)
    	close(f->fd_1);
    if (f->fd_2 != STDOUT_FILENO)
	{
        close(f->fd_2);
	}
    if (parser->delimeter != NULL && parser->redirection[0] == T_HEREDOC)
	{
        close(*heredoc_fd);
	}
	// if (status != 0 && is_builtin(parser))
	// 	kill(pid, SIGKILL);
}
