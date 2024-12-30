/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/11/24 22:47:28 by marvin            #+#    #+#             */
/*   Updated: 2024/11/24 22:47:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

t_parser* check_exit(t_parser *node)
{
	t_parser *parser;

	parser = node;
	while(parser)
	{
		if(strcmp(parser->command,"exit") == 0 && parser->next != NULL)
			return (parser -> next);
		parser = parser->next;
	}
	return (parser);
}
void	buitlin(t_parser *parser, t_env *env)
{
	if (strcmp(parser->command, "echo") == 0)
		global_var = builtin_echo(parser, env);
	else if (strcmp(parser->command, "env") == 0)
		global_var = builtin_env(parser, env);
	else if (strcmp(parser->command, "pwd") == 0)
		global_var = builtin_pwd(parser, env);
	else if (strcmp(parser->command, "export") == 0)
		global_var = builtin_export(parser, env);
	else if (strcmp(parser->command, "unset") == 0 && parser->next == NULL)
		builtin_unset(parser, env);
	else if (strcmp(parser->command, "cd") == 0 && parser->next == NULL)
		global_var = builtin_cd(parser, env);
	else if (strcmp(parser->command, "exit") == 0)
		builtin_exit(parser, env);
	
}

int	check_permissions(const char *filepath, int flag)
{
	struct stat file_stat;

	if (flag == 0 && stat(filepath, &file_stat) == -1)
	{
		printf("%s", filepath);
		perror("");
		return (0);
	}
	if (flag == 0 && !(file_stat.st_mode & S_IRUSR))
	{
		printf("Permission denied\n");
		return (0);
	}
	if (flag == 1 && stat(filepath, &file_stat) != -1)
	{
		if (!(file_stat.st_mode & S_IWUSR))
		{
			printf("Permission denied: Write permission not granted\n");
			return (0);
		}
	}
	return (1);
}

int	manage_redirection_output(t_parser *parser, int *fd)
{
	int i;
	int j;

	i = -1;
	j = -1;
	while (parser->redirection && parser->redirection[++i] != '\0')
	{
		if (parser->redirection[i] == T_OUTPUT
			|| parser->redirection[i] == T_APPEND)
		{
			if (parser->outfile != NULL && parser->outfile[++j] != NULL)
			{
				if (!check_permissions(parser->outfile[j], 1))
					return (global_var = 1, 1);
				if (j != 0)
					close(*fd);
				if (parser->redirection[i] == T_OUTPUT)
					*fd = open(parser->outfile[j], O_WRONLY | O_CREAT | O_TRUNC,
							0644);
				else if (parser->redirection[i] == T_APPEND)
					*fd = open(parser->outfile[j],
							O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
		}
	}
	return (0);
}

int	handle_input_output(t_parser *parser, t_fd *f, int fd[2], t_env *env)
{
	int val;

	if (parser->next != NULL)
		f->fd_2 = fd[1];
	if (parser->redirection)
	{
		if (parser->infile != NULL || parser->delimeter != NULL)
		{
			val = manage_redirection_input(parser, &f->fd_1,env);
			if (val == 1)
				return (0);
			else if (val == -1)
				return (-1);
		}
		if (parser->outfile != NULL)
		{
			val = manage_redirection_output(parser, &f->fd_2);
			if (val == 1)
				return (0);
			else if (val == -1)
				return (-1);
		}
	}
	return (1);
}