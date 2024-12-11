/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:56:23 by skreik            #+#    #+#             */
/*   Updated: 2024/12/11 13:00:37 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	execute_command(t_parser *parser, t_fd f, t_env *env)
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
		manage_input_output(heredoc_fd, &f);
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

void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env)
{
	pid_t	pid;
	int heredoc_fd;
		printf("parent pid %d\n",getpid());

	initialize_heredoc(&heredoc_fd, parser);
	//&&(strcmp(parser->command, "exit") == 0 || strcmp(parser->command, "cd") == 0)
	if (parser -> next == NULL)
		buitlin(parser, env);//ll exit
	
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0) // Child process
		{
			manage_input_output(-1, &f);
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

	while (parser)
	{
		if (parser->next)
			pipe(fd);
		input_redirection(parser, &f);
		output_redirection(parser, &f, fd);
		if (is_builtin(parser))
			execute_builtin_command(parser, f, env);
		else
			execute_command(parser, f, env);
		if (parser->next) // to handle input of cmd
		{
			close(fd[1]);
			f.fd_1 = fd[0];
		}
		parser = parser->next;
	}
	if (f.fd_1 != STDIN_FILENO)
		close(f.fd_1);
}
