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

#include"mini_shell.h"

void manage_T_input(t_parser *parser, int re, int *fd)
{
	if (re == T_INPUT)
	{
		if (parser->infile != NULL)
			*fd = open(parser->infile, O_RDONLY);
		else
			perror("Error: Input file not specified");
	}
}
void manage_T_output(t_parser *parser, int re, int *fd)
{
	int i;
	i = -1;

	if (re == T_OUTPUT)
	{
		if (parser->outfile != NULL)
		{
			while (parser->outfile[++i])
			{
				*fd = open(parser->outfile[i], O_WRONLY | O_CREAT | O_TRUNC,
						0644);
				if(parser->outfile[i + 1] != NULL)
					close(*fd);
			}
		}
		else
			perror("Error: Output file not specified");
	}

}
void manage_T_append(t_parser *parser, int re, int *fd)
{
	int i;
	if (re == T_APPEND)
	{
		if (parser->outfile != NULL)
		{
			i = -1;
			while (parser->outfile[++i])
				*fd = open(parser->outfile[i], O_WRONLY | O_CREAT | O_APPEND,
						0644);
		}
		else
			perror("Error: Output file not specified");
	}
}
int	ft_handle_redirections(t_parser *parser, int re)
{
	int	fd;

	fd = -1;
	manage_T_input(parser, re, &fd);
	manage_T_output(parser, re, &fd);
	manage_T_append(parser, re, &fd);
	if (fd == -1)
	{
		perror("Error opening file");
		// exit(1);
	}
	return (fd);
}

void buitlin(t_parser *parser, t_env *env)
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