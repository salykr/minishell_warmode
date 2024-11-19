/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:56:23 by skreik            #+#    #+#             */
/*   Updated: 2024/11/18 18:00:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	**ft_create_args(t_parser *parser)
{
	char	**path;
	int		i;

	path = NULL;
	path = add_string_to_2d_array(path, parser->command);
	i = 0;
	if (parser->operations != NULL)
	{
		while (parser->operations[i])
		{
			path = add_string_to_2d_array(path, parser->operations[i]);
			i++;
		}
	}
	i = 0;
	if (parser->input != NULL && parser->redirection == NULL)
	{
		while (parser->input[i])
		{
			printf("command : %s\n", parser->command);
			path = add_string_to_2d_array(path, parser->input[i]);
			i++;
		}
	}
	return (path);
}
char	*get_path(t_env env, char *cmd)
{
	int		i;
	char	*p;
	char	*back_slash;
	char	**path;
	char	*env_path;

	env_path = ft_getenv(&env, "PATH");
	if (!env_path)
		return (NULL);
	path = ft_split(env_path, ':');
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		back_slash = ft_strjoin("/", cmd);
		p = ft_strjoin(path[i], back_slash);
		free(back_slash);
		if (access(p, X_OK) == 0)
		{
			free_input(path); // Free the split path array
			return (p);
		}
		else
			free(p);
		i++;
	}
	free(path); // Free the split path array
	return (NULL);
}
char	*get_path_1(t_env env, char *cmd)
{
	char	*p;
	char	*env_path;

	env_path = ft_getenv(&env, "PWD");
	if (!env_path)
		return (NULL);
	p = ft_strjoin(env_path, &cmd[1]);
	printf("str: %s\n", p);
	if (access(p, X_OK) == 0)
		return (p);
	else
		free(p);
	return (NULL);
}
void	ft_redirection_delimiter(t_parser *node)
{
	char	*lineread;

	// signal(SIGINT, ctrl_c_press_here);
	while (1)
	{
		lineread = readline(">");
		if (!lineread) // If EOF is reached, break the loop
			break ;
		if (ft_strcmp(lineread, node->delimeter) == 0)
		{
			break ;
		}
		node->input = add_string_to_2d_array(node->input, lineread);
	}
	// signal(SIGINT, ctrl_c_press);
}

int	ft_handle_redirections(t_parser *parser, int re)
{
	int	fd;
	int	i;

	// this ft won't be called if there was no redirection
	fd = -1;
	i = 0;
	if (re == T_INPUT)
	{
		if (parser->infile != NULL)
			fd = open(parser->infile, O_RDONLY);
		else
			perror("Error: Input file not specified");
	}
	else if (re == T_OUTPUT)
	{
		if (parser->outfile != NULL)
		{
			while (parser->outfile[i])
			{
				fd = open(parser->outfile[i], O_WRONLY | O_CREAT | O_TRUNC,
						0644);
				i++;
			}
		}
		else
			perror("Error: Output file not specified");
	}
	else if (re == T_APPEND)
	{
		if (parser->outfile != NULL)
		{
			i = 0;
			while (parser->outfile[i])
			{
				fd = open(parser->outfile[i], O_WRONLY | O_CREAT | O_APPEND,
						0644);
				i++;
			}
		}
		else
			perror("Error: Output file not specified");
	}
	if (fd == -1)
	{
		perror("Error opening file");
		exit(1);
	}
	return (fd);
}

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

#include <string.h>

// int is_builtin(t_parser *parser)
// {
//     if (parser == NULL || parser->command == NULL)
//         return (0);
    
//     if (parser->operations != NULL && parser->operations[0] != NULL)
//     {
//         if (strcmp(parser->command, "echo") == 0)
//         {
//             if (strcmp(parser->operations[0], "-n") != 0)
//                     return (0); // Return 0 for invalid operations with echo
//             return (1); // Valid echo command with only -n option
//         }
//         return (0); // Return 0 for other commands with operations
//     }
//     if (strcmp(parser->command, "cd") == 0 ||
//         strcmp(parser->command, "pwd") == 0 ||
//         strcmp(parser->command, "export") == 0 ||
//         strcmp(parser->command, "unset") == 0 ||
//         strcmp(parser->command, "env") == 0 ||
//         strcmp(parser->command, "exit") == 0 ||
//         strcmp(parser->command, "echo") == 0)
//     {
//         return (1); // Valid built-in command
//     }

//     return (0); // Not a built-in command
// }

int	handle_heredoc(char **heredoc_content)
{
	int	pipefd[2];
	int	i;

	i = 0;
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		exit(EXIT_FAILURE);
	}
	while (heredoc_content[i] != NULL)
	{
		write(pipefd[1], heredoc_content[i], strlen(heredoc_content[i]));
		write(pipefd[1], "\n", 1);
		i++;
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
void	execute_command(t_parser *parser, t_fd f, t_env *env)
{
	char	*cmd_path;
	char	**args;
	int		heredoc_fd;
	pid_t	pid;

	heredoc_fd = -1;
	if (parser->delimeter != NULL && parser->redirection != NULL)
	{
		if (parser->redirection[0] == T_HEREDOC)
			heredoc_fd = handle_heredoc(parser->input);
	}
	if (!strncmp(parser->command, "./", 2))
		cmd_path = get_path_1(*env, parser->command);
	else
	{
		cmd_path = get_path(*env, parser->command);
	}
	args = ft_create_args(parser);
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (heredoc_fd != -1)
		{
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		else if (f.fd_1 != STDIN_FILENO)
		{
			dup2(f.fd_1, STDIN_FILENO); // Redirect input
			close(f.fd_1);
		}
		if (f.fd_2 != STDOUT_FILENO)
		{
			dup2(f.fd_2, STDOUT_FILENO); // Redirect output
			close(f.fd_2);
		}
		execve(cmd_path, args, env->env);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		free_input(args);
		free(cmd_path);
		if (f.fd_2 != STDOUT_FILENO)
			close(f.fd_2);
		if (parser->delimeter != NULL && parser->redirection[0] == T_HEREDOC)
			close(heredoc_fd);
	}
}
void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env)
{
	pid_t	pid;
	int		status;
	if (strcmp(parser->command, "unset") == 0 && parser->next == NULL)
	{
		builtin_unset(parser->input, env);
		return ;
	}
	else if (strcmp(parser->command, "cd") == 0 && parser->next == NULL)
	{
		if(builtin_cd(parser, env)==-1)
			global_var = 1;
		else
			global_var = 0;
		return ;
	}
	else if (strcmp(parser->command, "exit") == 0 && parser->next == NULL)
	{
		builtin_exit(parser, env);
		return ;
	}
	else if (strcmp(parser->command, "export") == 0 && parser->next == NULL)
	{
		global_var = builtin_export(parser, env); // Update environment variables
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) // Child process
	{
		if (f.fd_1 != STDIN_FILENO)
		{
			dup2(f.fd_1, STDIN_FILENO);
			close(f.fd_1);
		}
		if (f.fd_2 != STDOUT_FILENO)
		{
			dup2(f.fd_2, STDOUT_FILENO);
			close(f.fd_2);
		}
		// Execute the built-in command
		if (strcmp(parser->command, "echo") == 0)
			builtin_echo(parser, *env);
		else if (strcmp(parser->command, "env") == 0)
			builtin_env(parser, env);
		else if (strcmp(parser->command, "pwd") == 0)
			builtin_pwd(parser, env); 
		else if (strcmp(parser->command, "export") == 0)
			global_var = builtin_export(parser, env);
		printf("in child :%d\n",global_var);
		// Exit the child process after execution
		exit(EXIT_SUCCESS);
	}
	else // Parent process
	{
		waitpid(pid, &status, 0);
		global_var = status;
		printf("var: %d\n",global_var);
		if (f.fd_2 != STDOUT_FILENO)
			close(f.fd_2);
		if (status != 0)
			kill(pid, SIGKILL);
	}
}

void	cmds_exec(t_parser *parser, t_env *env)
{
	t_fd f;
	int fd[2];
	int status;
	// int i = 0;
	t_parser *list = parser;
	f.fd_1 = STDIN_FILENO;

	while (parser)
	{
		if (parser->next)
			pipe(fd);
		if (parser->prev == NULL)
		{
			if (parser->input == NULL && parser->delimeter != NULL
				&& parser->redirection[0] == T_HEREDOC)
			{
				// f.fd_1 = -10;
				f.fd_1 = STDIN_FILENO;
				ft_redirection_delimiter(parser);
				// printf("inpt taken\n");
			}
			if (parser->infile != NULL && parser->redirection != NULL
				&& (parser->redirection[0] == T_INPUT
					|| parser->redirection[0] == T_HEREDOC))
			{
				f.fd_1 = ft_handle_redirections(parser, parser->redirection[0]);
				printf("fd_1: %d\n", f.fd_1);
			}
		}
		if (parser->next == NULL) // to handle output of cmd
		{
			if (parser->outfile == NULL)
				f.fd_2 = STDOUT_FILENO;
			else if (parser->outfile != NULL && parser->redirection != NULL
				&& ((parser->redirection[0] == T_OUTPUT
						|| parser->redirection[0] == T_APPEND)
					|| (parser->redirection[1] != '\0'
						&& (parser->redirection[1] == T_OUTPUT
							|| parser->redirection[1] == T_APPEND))))
			{
				if (parser->redirection[1] != '\0')
					f.fd_2 = ft_handle_redirections(parser,
							parser->redirection[1]);
				else
					f.fd_2 = ft_handle_redirections(parser,
							parser->redirection[0]);
				printf("fd_2: %d\n", f.fd_2);
			}
		}
		else
			f.fd_2 = fd[1];
		if (is_builtin(parser))
		{
			printf("~~~~~~builtin-~~~~~~~\n");
			printf("before executing echo: %d\n", global_var);
			execute_builtin_command(parser, f, env);
			printf("after executing echo: %d\n", global_var);
		}
		else
		{
			printf("------not builtin-------\n");
			execute_command(parser, f, env);
		}
		if (parser->next) // to handle input of cmd
		{
			close(fd[1]);
			f.fd_1 = fd[0];
		}
		parser = parser->next;
		// wait(NULL);
	}
	if (f.fd_1 != STDIN_FILENO)
		close(f.fd_1);
	while (list)
	{
		wait(&status);
		env->exit_code = status;
		list = list->next;
	}
}