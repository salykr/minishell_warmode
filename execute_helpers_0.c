/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_0.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:08:51 by marvin            #+#    #+#             */
/*   Updated: 2024/11/24 15:08:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"


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
			return (free_input(path), p);
		else
			free(p);
		i++;
	}
	return (free_input(path), NULL);
}
char	*get_path_PWD(t_env env, char *cmd)
{
	char	*p;
	char	*env_path;

	env_path = ft_getenv(&env, "PWD");
	if (!env_path)
		return (NULL);
	p = ft_strjoin(env_path, &cmd[1]);
	if (access(p, X_OK) == 0)
		return (p);
	else
	{
		printf("bash: %s: Permission denied\n",p);
		free(p);
	}
	return (NULL);
}
//ft_redirection_delimiter
void free_heredoc(char **input_array)
{
	char **ptr;

	ptr = input_array;
	if (!input_array)
		return;
	while (*ptr)
	{
		free(*ptr);
		ptr++;
	}
	free(input_array);
	*input_array = NULL;
}
void write_in_heredoc(t_parser *node)
{
    char *line;
	size_t line_len;
	int i;
	int j;
	j = 0;
    set_signal_handler(ctrl_c_press_heredoc);
    // handle_child_signals(ctrl_c_press_heredoc);
	line = NULL;
    line_len = 0;
	global_var = 0;
	i = 0;
	while(node->delimeter != NULL && node->delimeter[i] != NULL && global_var != 130 )
	{
		printf("in OUTER loop for %dth time global var %d \n",j, global_var);
		if(node->heredoc != NULL)
		{
			free_input(node->heredoc);
			node->heredoc = NULL;
		}
		while(1)
		{
			printf("in  loop for %dth time global var %d \n",j, global_var);
			write(STDOUT_FILENO, "> ", 2); // Write the prompt directly to stdout
			line = get_next_line(0); // Read input from stdin
			if (!line) // Handle EOF or error
				break;
			line_len = strlen(line);
			if (line[line_len - 1] == '\n')
				line[line_len - 1] = '\0';
			if (strcmp(line, node->delimeter[i]) == 0)
			{
				free(line);
				break;
			}
			node->heredoc = add_string_to_2d_array(node->heredoc, line);
			free(line); // Free the line buffer after processing
			j++;
		}
		i++;
	}
	if (global_var == 130)
	{
		free_input(node->heredoc);
		node->heredoc = NULL;
	}
	set_signal_handler(ctrl_c_press);
	// setup_signal_handlers();
	// if(global_var)
	// printf("global var is: %d\n",global_var);
}

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
	while (heredoc_content != NULL && heredoc_content[i] != NULL)
	{
		write(pipefd[1], heredoc_content[i], strlen(heredoc_content[i]));
		write(pipefd[1], "\n", 1);
		i++;
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

