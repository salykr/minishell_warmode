/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:56:23 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/21 13:12:04 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

// void	execute_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
// {
// 	char	*cmd_path;
// 	pid_t	pid;

// 	initialize_execution(parser, env, &cmd_path);
// 	pid = fork();
// 	if (pid == -1)
// 		exit(EXIT_FAILURE);
// 	if (parser->prev && parser->prev->permission == 1)
// 		return (g_v = 0, free(cmd_path));
// 	if (pid == 0)
// 	{
// 		configure_child_signals();
// 		manage_input_output(&f, fd, parser);
// 		if ((cmd_path != NULL))
// 		{
// 			process_dollar_strings(parser->args, env);
// 			if (execve(cmd_path, parser->args, env->env) == -1)
// 				perror("execve");
// 		}
// 		else
// 			ft_putendl_fd("Error: PATH environment variable not set", 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	else
// 		return (ignore_signals(), handle_child_exit(&f), free(cmd_path));
// }

// void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
// {
// 	pid_t	pid;
// 	t_fd	original_fds;

// 	if (parser->next == NULL && parser->prev == NULL)
// 	{
// 		if (save_original_fds(&original_fds.fd_1, &original_fds.fd_2) == -1)
// 			return ;
// 		manage_input_output(&f, fd, parser);
// 		buitlin(parser, env);
// 		restore_original_fds(original_fds.fd_1, original_fds.fd_2);
// 	}
// 	else
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 			exit(EXIT_FAILURE);
// 		if (pid == 0)
// 		{
// 			manage_input_output(&f, fd, parser);
// 			buitlin(parser, env);
// 			exit(g_v);
// 		}
// 		else
// 			handle_child_exit(&f);
// 	}
// }

// void	handle_wait_status(t_parser *parser)
// {
// 	int	status;

// 	while (wait(&status) > 0)
// 	{
// 		if (WIFSIGNALED(status))
// 			g_v = WTERMSIG(status) + 128;
// 		else
// 		{
// 			if (parser && parser->next == NULL)
// 				g_v = WEXITSTATUS(status);
// 			else
// 				g_v = WEXITSTATUS(status);
// 		}
// 		if (parser && parser->next == NULL)
// 			g_v = WEXITSTATUS(status);
// 	}
// }

// int	handle_io_and_execute(t_parser *parser, t_env *env, t_fd *f, int fd[2])
// {
// 	int	val;

// 	val = handle_input_output(parser, f, fd, env);
// 	if (val == 0)
// 		return (0);
// 	else if (val == -1)
// 	{
// 		g_v = 130;
// 		return (-1);
// 	}
// 	if (is_builtin(parser))
// 		execute_builtin_command(parser, *f, env, fd);
// 	else
// 		execute_command(parser, *f, env, fd);
// 	return (1);
// }

// void	cmds_exec(t_parser *parser, t_env *env)
// {
// 	t_fd	f;
// 	int		fd[2];

// 	f.fd_1 = STDIN_FILENO;
// 	process_parser_files(parser, env);
// 	while (parser)
// 	{
// 		if (parser->next)
// 			pipe(fd);
// 		f.fd_2 = STDOUT_FILENO;
// 		if (handle_io_and_execute(parser, env, &f, fd) != 1
// 			|| parser->permission == 1)
// 		{
// 			parser = parser->next;
// 			continue ;
// 		}
// 		close_file(parser, fd, &f);
// 		parser = parser->next;
// 	}
// 	if (f.fd_1 != STDIN_FILENO)
// 		close(f.fd_1);
// 	handle_wait_status(parser);
// 	restore_signals();
// }

void	execute_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	char	*cmd_path;
	pid_t	pid;

	initialize_execution(parser, env, &cmd_path);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (parser->prev && parser->prev->permission == 1)
		return (g_v = 0, free(cmd_path));
	if (pid == 0)
	{
		configure_child_signals();
		manage_input_output(&f, fd, parser);
		if ((cmd_path != NULL))
		{
			process_dollar_strings(parser->args, env);
			if (execve(cmd_path, parser->args, env->env) == -1)
				perror("execve");
		}
		else
			ft_putendl_fd("Error: PATH environment variable not set", 2);
		exit(EXIT_FAILURE);
	}
	else
		return (ignore_signals(), handle_child_exit(&f), free(cmd_path));
}

void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	pid_t	pid;
	t_fd	original_fds;

	if (parser->next == NULL && parser->prev == NULL)
	{
		if (save_original_fds(&original_fds.fd_1, &original_fds.fd_2) == -1)
			return ;
		manage_input_output(&f, fd, parser);
		buitlin(parser, env);
		restore_original_fds(original_fds.fd_1, original_fds.fd_2);
	}
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0)
		{
			manage_input_output(&f, fd, parser);
			buitlin(parser, env);
			exit(g_v);
		}
		else
			handle_child_exit(&f);
	}
}

void	handle_wait_status(t_parser *parser, t_env *env)
{
	int	status;
	int	signal;

	while (wait(&status) > 0)
	{
		if (WIFSIGNALED(status))
		{	
			signal = WTERMSIG(status);
			if (signal == SIGINT || signal == SIGQUIT)
				env->exit_code = signal + 128;
		}
		else
		{
			if (parser && parser->next == NULL)
				env->exit_code = WEXITSTATUS(status);
			else
				env->exit_code = WEXITSTATUS(status);
		}
		if (parser && parser->next == NULL)
			env->exit_code = WEXITSTATUS(status);
	}
	g_v = 0;
}

int	handle_io_and_execute(t_parser *parser, t_env *env, t_fd *f, int fd[2])
{
	int	val;

	val = handle_input_output(parser, f, fd, env);
	if (val == 0)
		return (0);
	else if (val == -1)
	{
		env->exit_code = 130;
		g_v = 0;
		return (-1);
	}
	if (is_builtin(parser))
		execute_builtin_command(parser, *f, env, fd);
	else
		execute_command(parser, *f, env, fd);
	return (1);
}

void	cmds_exec(t_parser *parser, t_env *env)
{
	t_fd	f;
	int		fd[2];

	f.fd_1 = STDIN_FILENO;
	process_parser_files(parser, env);
	while (parser)
	{
		if (parser->next)
			pipe(fd);
		f.fd_2 = STDOUT_FILENO;
		if (handle_io_and_execute(parser, env, &f, fd) != 1
			|| parser->permission == 1)
		{
			parser = parser->next;
			continue ;
		}
		close_file(parser, fd, &f);
		parser = parser->next;
	}
	if (f.fd_1 != STDIN_FILENO)
		close(f.fd_1);
	handle_wait_status(parser, env);
	restore_signals();
}
