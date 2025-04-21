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

#include "mini_shell.h"

int	is_builtin(t_parser *parser)
{
	if (parser->command == NULL)
		return (0);
	if (ft_strcmp(parser->command, "cd") == 0 || ft_strcmp(parser->command,
			"pwd") == 0 || ft_strcmp(parser->command, "export") == 0
		|| ft_strcmp(parser->command, "unset") == 0
		|| ft_strcmp(parser->command, "env") == 0
		|| ft_strcmp(parser->command, "exit") == 0
		|| ft_strcmp(parser->command, "echo") == 0)
	{
		return (1);
	}
	else if (!access(parser->command, X_OK)
		&& (ft_strstr(parser->command, "cd") != NULL
			|| ft_strstr(parser->command, "pwd") != NULL
			|| ft_strstr(parser->command, "export") != NULL
			|| ft_strstr(parser->command, "unset") != NULL
			|| ft_strstr(parser->command, "env") != NULL
			|| ft_strstr(parser->command, "exit") != NULL
			|| ft_strstr(parser->command, "echo") != NULL))
	{
		return (1);
	}
	return (0);
}

void	manage_input_output(t_fd *f, int fd[2], t_parser *parser)
{
	if (f->fd_1 != STDIN_FILENO)
	{
		dup2(f->fd_1, STDIN_FILENO);
		close(f->fd_1);
	}
	if (f->fd_2 != STDOUT_FILENO)
	{
		dup2(f->fd_2, STDOUT_FILENO);
		close(f->fd_2);
	}
	if (parser->next != NULL)
		close(fd[0]);
}

void	initialize_execution(t_parser *parser, t_env *env, char **cmd_path)
{
	if (parser->command && !ft_strncmp(parser->command, "./", 2))
		*cmd_path = get_path_pwd(env, parser->command);
	else if (parser->command && ft_strchr(parser->command, '/')
		&& !access(parser->command, X_OK))
		*cmd_path = ft_strdup(parser->command);
	else
		*cmd_path = get_path(*env, parser->command);
}

void	handle_child_exit(t_fd *f)
{
	if (f->fd_2 != STDOUT_FILENO)
		close(f->fd_2);
	if (f->fd_1 != STDIN_FILENO)
		close(f->fd_1);
}

void	close_file(t_parser *parser, int fd[2], t_fd *f)
{
	if (parser->next)
	{
		close(fd[1]);
		f->fd_1 = fd[0];
	}
}
