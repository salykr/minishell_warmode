/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:56:23 by skreik            #+#    #+#             */
/*   Updated: 2024/12/20 13:55:33 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	execute_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	char	*cmd_path;
	char	**args;
	int		heredoc_fd;
	pid_t	pid;


	args = initialize_execution(&heredoc_fd, parser, env, &cmd_path);
	printf("args\n");
	print_2d_array(args);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		manage_input_output(heredoc_fd, &f, fd, parser);
		if(ft_getenv(env, "PATH")!=NULL)
			execve(cmd_path, args, env->env);
		perror("Error");
		exit(EXIT_FAILURE);
	}
	else
	{
		handle_child_exit(pid, &heredoc_fd, &f, parser);
		free_input(args);
		free(cmd_path);
	}
}

void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	pid_t	pid;
	int heredoc_fd;
	printf("parent pid %d\n",getpid());

	initialize_heredoc(&heredoc_fd, parser);
	if(parser->next == NULL)
	{
		buitlin(parser, env);//ll exit
				manage_input_output(-1, &f,fd, parser);

	}
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0) // Child process
		{
			manage_input_output(-1, &f,fd, parser);
			buitlin(parser, env);
			exit(global_var);
		}
		else // Parent process
			handle_child_exit(pid, &heredoc_fd, &f, parser);
			
	}
}

void input_redirection(t_parser *parser, t_fd *f)
{
	if (parser->prev == NULL)
		{
			if (parser->input == NULL && parser->delimeter != NULL
				&& parser->redirection[0] == T_HEREDOC)
				ft_redirection_delimiter(parser);
			if (parser->infile != NULL && parser->redirection != NULL
				&& (parser->redirection[0] == T_INPUT
					|| parser->redirection[0] == T_HEREDOC))
				f->fd_1 = ft_handle_redirections(parser, parser->redirection[0]);
		}
}
void output_redirection(t_parser *parser, t_fd *f, int fd[2])
{
	if (parser->next == NULL) // to handle output of cmd
		{
			if (parser->outfile == NULL)
				f->fd_2 = STDOUT_FILENO;
			else if (parser->outfile != NULL && parser->redirection != NULL
				&& ((parser->redirection[0] == T_OUTPUT
						|| parser->redirection[0] == T_APPEND)
					|| (parser->redirection[1] != '\0'
						&& (parser->redirection[1] == T_OUTPUT
							|| parser->redirection[1] == T_APPEND))))
			{
				if (parser->redirection[1] != '\0')
					f->fd_2 = ft_handle_redirections(parser,
							parser->redirection[1]);
				else
					f->fd_2 = ft_handle_redirections(parser,
							parser->redirection[0]);
			}
		}
	else
		f->fd_2 = fd[1];
}

void	cmds_exec(t_parser *parser, t_env *env)
{
	t_fd f;
	int fd[2];
	f.fd_1 = STDIN_FILENO;
	int status;
	fd[0] = -1;
	fd[1] = -1;

	while (parser)
	{
		if (parser->next)
			pipe(fd);
		input_redirection(parser, &f);
		output_redirection(parser, &f, fd);
		if (is_builtin(parser))
			execute_builtin_command(parser, f, env, fd);
		else
			execute_command(parser, f, env, fd);
		if (parser->next) // to handle input of cmd
		{
			close(fd[1]);
			f.fd_1 = fd[0];
		}
		parser = parser->next;
	}
	if (f.fd_1 != STDIN_FILENO)
		close(f.fd_1);
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			global_var = WEXITSTATUS(status);
	}

}
// void cmds_exec(t_parser *parser, t_env *env)
// {
//     t_fd f;
//     int fd[2];
//     int status;
//     pid_t pid;

//     f.fd_1 = STDIN_FILENO;

//     while (parser)
//     {
//         if (parser->next && pipe(fd) == -1)
//         {
//             perror("Pipe failed");
//             exit(EXIT_FAILURE);
//         }

//         pid = fork();
//         if (pid == -1)
//         {
//             perror("Fork failed");
//             exit(EXIT_FAILURE);
//         }

//         if (pid == 0) // Child process
//         {
//             if (parser->next)
//             {
//                 close(fd[0]);            // Close read end for child
//                 dup2(fd[1], STDOUT_FILENO); // Redirect output to pipe
//                 close(fd[1]);
//             }
//             if (f.fd_1 != STDIN_FILENO)
//             {
//                 dup2(f.fd_1, STDIN_FILENO); // Redirect input
//                 close(f.fd_1);
//             }

//             if (is_builtin(parser))
//                 buitlin(parser, env);
//             else
//                 execute_command(parser, f, env);
//             exit(EXIT_SUCCESS);
//         }
//         else // Parent process
//         {
//             if (f.fd_1 != STDIN_FILENO)
//                 close(f.fd_1); // Close input fd for parent

//             if (parser->next)
//             {
//                 close(fd[1]);    // Close write end in parent
//                 f.fd_1 = fd[0];  // Set read end for next command
//             }
//         }
//         parser = parser->next;
//     }

//     while (wait(&status) > 0); // Wait for all child processes to terminate
// }

