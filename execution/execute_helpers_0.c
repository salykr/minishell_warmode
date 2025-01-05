/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_0.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+
						      +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+
     +#+        */
/*                                                +#+#+#+#+#+
						  +#+           */
/*   Created: 2024/11/24 15:08:51 by marvin            #+#    #+#             */
/*   Updated: 2024/11/24 15:08:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*get_path(t_env env, char *cmd)
{
	int			i;
	char		*p;
	char		*back_slash;
	char		**path;
	char		*env_path;

	env_path = retreive_path(env);
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
			return (free_2d_array(path), p);
		else
			free(p);
		i++;
	}
	return (free_2d_array(path), NULL);
}

char	*get_path_pwd(t_env env, char *cmd)
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
		printf("bash: %s: Permission denied\n", p);
		free(p);
	}
	return (NULL);
}

void	handle_input_line(t_parser *node, int delimiter_index)
{
	char	*line;
	char	*ptr;
	size_t	line_len;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(0);
		if (!line)
			break ;
		line_len = strlen(line);
		if (line[line_len - 1] == '\n')
			line[line_len - 1] = '\0';
		ptr = remove_quotes(node->delimeter[delimiter_index]);
		if (strcmp(line, ptr) == 0)
		{
			handle_memory_errors(line, ptr);
			break ;
		}
		node->heredoc = add_string_to_2d_array(node->heredoc, line);
		handle_memory_errors(line, ptr);
	}
}

void	write_in_heredoc(t_parser *node)
{
	int	i;

	g_v = 0;
	set_signal_handler_heredoc();
	i = -1;
	while (node->delimeter != NULL && node->delimeter[++i] != NULL
		&& g_v != 130)
	{
		if (node->heredoc != NULL)
			free_heredoc(node);
		handle_input_line(node, i);
	}
	if (g_v == 130)
		free_heredoc(node);
	restore_signals();
}

int	handle_heredoc(char **heredoc_content, t_env *env)
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
		process_dollar_strings(heredoc_content, env);
		write(pipefd[1], (heredoc_content[i]), strlen(heredoc_content[i]));
		write(pipefd[1], "\n", 1);
		i++;
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
