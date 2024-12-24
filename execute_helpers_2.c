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
	int i =0;
	if (re == T_INPUT)
	{
		if (parser->infile != NULL)
		{
			while (parser->infile[i])
			{
				*fd = open(parser->infile[i], O_RDONLY);
				if(parser->infile[i + 1] != NULL)
				{
					printf("closeeeeeeeeeeee\n");
					close(*fd);
				}
				// printf("infile: %s\n",parser->infile[i]);
				i++;
				// cat < file1.txt < file2.txt > combined_output.txt
			}
		}
	}
	printf("infile at end: %s fd: %d\n",parser->infile[i - 1], *fd);
}
// void manage_OUTPUT(t_parser *parser, int re, int *fd)
// {
	
// }
void manage_T_output(t_parser *parser, int re, int *fd)
{
	int i;
	i = 0;
	 printf("outfile now\nbgrgrg\n");
	if (re == T_OUTPUT)
	{
		if (parser->outfile != NULL)
		{
			while (parser->outfile[i])
			{
				*fd = open(parser->outfile[i], O_WRONLY | O_CREAT | O_TRUNC,
						0644);
				if(parser->outfile[i + 1] != NULL)
					close(*fd);
				i++;
			}
		}
		else
			perror("Error: Output file not specified");
	}
	// printf("IGGGGGG????\n");

	// // if(i - 1 == 0)
	//  	printf("outfile at end: %s fd: %d\n",parser->outfile[0], *fd);
	// else
	// 	printf("outfile at end: %s fd: %d\n",parser->outfile[i - 1], *fd);

}



void manage_input(t_parser *parser, int *fd)
{
	int i =0;
		if (parser->infile != NULL)
		{
			while (parser->infile[i])
			{
				*fd = open(parser->infile[i], O_RDONLY);
				if(parser->infile[i + 1] != NULL)
				{
					// printf("closeeeeeeeeeeee\n");
					close(*fd);
				}
				// printf("infile: %s\n",parser->infile[i]);
				i++;
				// cat < file1.txt < file2.txt > combined_output.txt
			}
		}
	printf("the nb is %d\n",*fd);
}
// void manage_OUTPUT(t_parser *parser, int re, int *fd)
// {
	
// }
void manage_output(t_parser *parser, int *fd)
{
	int i;
	i = 0;

		if (parser->outfile != NULL)
		{
			while (parser->outfile[i])
			{
				*fd = open(parser->outfile[i], O_WRONLY | O_CREAT | O_TRUNC,
						0644);
				if(parser->outfile[i + 1] != NULL)
					close(*fd);
				i++;
			}
		}
		else
			perror("Error: Output file not specified");
	// printf("IGGGGGG????\n");

	// // if(i - 1 == 0)
	//  	printf("outfile at end: %s fd: %d\n",parser->outfile[0], *fd);
	// else
	// 	printf("outfile at end: %s fd: %d\n",parser->outfile[i - 1], *fd);

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
int	ft_handle_redirections(t_parser *parser)
{
	int	fd;

	fd = -1;
	// manage_T_input(parser, re, &fd);
	// manage_T_output(parser, re, &fd);
	// manage_T_append(parser, re, &fd);
	manage_input(parser, &fd);
	manage_output(parser, &fd);
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