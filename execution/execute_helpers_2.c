/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:47:28 by marvin            #+#    #+#             */
/*   Updated: 2024/11/24 22:47:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

t_parser	*check_exit(t_parser *node)
{
	t_parser	*parser;

	parser = node;
	while (parser)
	{
		if (ft_strcmp(parser->command, "exit") == 0 && parser->next != NULL)
			return (parser -> next);
		parser = parser->next;
	}
	return (parser);
}

void	buitlin(t_parser *parser, t_env *env)
{
	int		val;

	val = get_variable(parser, env);
	if (ft_strcmp(parser->command, "echo") == 0)
		env->exit_code = builtin_echo(parser, env);
	else if ((ft_strcmp(parser->command, "env") == 0 && !val)
		|| ft_strstr(parser->command, "env") != NULL)
		env->exit_code = builtin_env(parser, env);
	else if (ft_strstr(parser->command, "pwd") != NULL)
		env->exit_code = builtin_pwd(parser, env);
	else if (ft_strstr(parser->command, "export") != NULL)
		env->exit_code = builtin_export(parser, env);
	else if (ft_strstr(parser->command, "unset") != NULL)
		builtin_unset(parser, env);
	else if (ft_strstr(parser->command, "cd") != NULL)
		env->exit_code = builtin_cd(parser, env);
	else if (ft_strstr(parser->command, "exit") != NULL)
		builtin_exit(parser, env);
	g_v = 0;
}

int	check_permissions(const char *filepath, int flag, t_parser *parser,
			t_env *env)
{
	struct stat	file_stat;

	parser->permission = 0;
	if (flag == 0 && stat(filepath, &file_stat) == -1)
	{
		ft_putendl_fd("bash: No such file or directory", 2);
		env->exit_code = 1;
		parser->permission = 1;
		return (0);
	}
	if (flag == 0 && !(file_stat.st_mode & S_IRUSR))
	{
		ft_putendl_fd("Permission denied", 2);
		parser->permission = 1;
		return (0);
	}
	if (flag == 1 && stat(filepath, &file_stat) != -1)
	{
		if (!(file_stat.st_mode & S_IWUSR))
		{
			parser->permission = 1;
			return (ft_putendl_fd("Write permission not granted", 2), 0);
		}
	}
	return (1);
}

int	manage_redirection_output(t_parser *parser, int *fd, t_env *env)
{
	t_ij	ij;

	ij.i = -1;
	ij.j = -1;
	while (parser->redirection && parser->redirection[++ij.i] != '\0')
	{
		if (parser->redirection[ij.i] == T_OUTPUT
			|| parser->redirection[ij.i] == T_APPEND)
		{
			if (parser->outfile != NULL && parser->outfile[++ij.j] != NULL)
			{
				if (!check_permissions(parser->outfile[ij.j], 1, parser, env))
					return (env->exit_code = 1, 1);
				if (ij.j != 0)
					close(*fd);
				if (parser->redirection[ij.i] == T_OUTPUT)
					*fd = open(parser->outfile[ij.j],
							O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else if (parser->redirection[ij.i] == T_APPEND)
					*fd = open(parser->outfile[ij.j],
							O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
		}
	}
	return (0);
}

int	handle_input_output(t_parser *parser, t_fd *f, int fd[2], t_env *env)
{
	int	val;

	if (parser->next != NULL)
		f->fd_2 = fd[1];
	if (parser->redirection)
	{
		if (parser->infile != NULL || parser->delimeter != NULL)
		{
			val = manage_redirection_input(parser, &f->fd_1, env);
			if (val == 1)
				return (0);
			else if (val == -1)
				return (-1);
		}
		if (parser->outfile != NULL)
		{
			val = manage_redirection_output(parser, &f->fd_2, env);
			if (val == 1)
				return (0);
			else if (val == -1)
				return (-1);
		}
	}
	return (1);
}
