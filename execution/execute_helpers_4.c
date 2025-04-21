/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:56:32 by marvin            #+#    #+#             */
/*   Updated: 2024/12/29 17:56:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	check_heredoc_existence(int *redirection)
{
	int	i;

	i = 0;
	while (redirection[i] != '\0')
	{
		if (redirection[i] == T_HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

int	save_original_fds(int *original_stdin, int *original_stdout)
{
	*original_stdin = dup(STDIN_FILENO);
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdin == -1 || *original_stdout == -1)
	{
		perror("Error saving original file descriptors");
		return (-1);
	}
	return (0);
}

void	restore_original_fds(int original_stdin, int original_stdout)
{
	if (dup2(original_stdin, STDIN_FILENO) == -1)
		perror("Error restoring STDIN");
	if (dup2(original_stdout, STDOUT_FILENO) == -1)
		perror("Error restoring STDOUT");
	close(original_stdin);
	close(original_stdout);
}

void	free_heredoc(t_parser *node)
{
	if (node && node->heredoc)
	{
		free_2d_array(node->heredoc);
		node->heredoc = NULL;
	}
}

t_parser	*find_last_exit(t_parser *parser)
{
	t_parser	*head;
	t_parser	*last_node;

	head = parser;
	last_node = NULL;
	while (parser)
	{
		if (parser->command)
		{
			if (ft_strcmp(parser->command, "exit") == 0)
				last_node = parser;
		}
		parser = parser->next;
	}
	if (last_node != NULL)
		return (last_node->next);
	return (head);
}
