/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:56:23 by skreik            #+#    #+#             */
/*   Updated: 2024/12/23 17:50:43 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	execute_command(t_parser *parser, t_fd f, t_env *env, int fd[2], t_tokenlist *token)
{
	char	*cmd_path;
	char	**args;
	int		heredoc_fd;
	pid_t	pid;
	(void)fd;

	args = initialize_execution(&heredoc_fd, parser, env, &cmd_path, token);
	printf("args\n");
	print_2d_array(args);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		manage_input_output(heredoc_fd, &f);
		if(f.fd_1 == -1 ||  f.fd_2 == -1)
			return;
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

int save_original_fds(int *original_stdin, int *original_stdout) {
    *original_stdin = dup(STDIN_FILENO);
    *original_stdout = dup(STDOUT_FILENO);
    if (*original_stdin == -1 || *original_stdout == -1) {
        perror("Error saving original file descriptors");
        return -1;
    }
    return 0;
}
void restore_original_fds(int original_stdin, int original_stdout) {
    if (dup2(original_stdin, STDIN_FILENO) == -1) {
        perror("Error restoring STDIN");
    }
    if (dup2(original_stdout, STDOUT_FILENO) == -1) {
        perror("Error restoring STDOUT");
    }
    close(original_stdin);
    close(original_stdout);
}

void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	pid_t	pid;
	int heredoc_fd;
	printf("parent pid %d\n",getpid());
	int original_stdin, original_stdout;
	(void)fd;
	
	initialize_heredoc(&heredoc_fd, parser);
 	if (parser->next == NULL) 
	{
        if (save_original_fds(&original_stdin, &original_stdout) == -1)
            return; // Handle error
        manage_input_output(-1, &f);
		if(f.fd_1 == -1 ||  f.fd_2 == -1)
			return;
		buitlin(parser, env);
        restore_original_fds(original_stdin, original_stdout);
    } 
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0) // Child process
		{
			manage_input_output(-1,&f);
			if(f.fd_1 == -1 ||  f.fd_2 == -1)
			{
				exit(EXIT_FAILURE);
			}
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
			if (parser->infile != NULL && parser->redirection != NULL)
				f->fd_1 = ft_handle_redirections(parser);
		}
}
void output_redirection(t_parser *parser, t_fd *f, int fd[2])
{
	if (parser->next == NULL) // to handle output of cmd
		{
			if (parser->outfile == NULL)
				f->fd_2 = STDOUT_FILENO;
			else if (parser->outfile != NULL && parser->redirection != NULL)
			{
				if (parser->redirection[1] != '\0')
					f->fd_2 = ft_handle_redirections(parser);
				else
					f->fd_2 = ft_handle_redirections(parser);
			}
		}
	else
		f->fd_2 = fd[1];
}

void	cmds_exec(t_parser *parser, t_env *env, t_tokenlist *token_list)
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
			printf("in cmds_exec : fd_1: %d\n fd_ 2:%d\nss",f.fd_1,f.fd_2);

		if (is_builtin(parser))
			execute_builtin_command(parser, f, env, fd);
		else
			execute_command(parser, f, env, fd, token_list);
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

