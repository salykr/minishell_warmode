/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_fts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 12:05:09 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/12 14:34:32 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

t_env	*initialize_environment(char **envp)
{
	if (envp == NULL || *envp == NULL)
		return (manage_env_i());
	else
		return (init_env(envp));
}

int	ft_manage_empty_input(char *line)
{
	if (ft_strlen(line) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

int	handle_tokenization_error(t_tokenlist *token_list, char *line)
{
	if (!token_list)
	{
		ft_putendl_fd("Error: Failed to tokenize input.", 2);
		free(line);
		return (1);
	}
	return (0);
}

int	handle_parser_error(t_parser *parser, t_tokenlist *token_list, char *line)
{
	if (!parser)
	{
		ft_putendl_fd("Error: Failed to create parser.", 2);
		free(line);
		free_token_list(token_list);
		return (1);
	}
	return (0);
}

void	cleanup_resources(char *line, t_tokenlist *token_list, t_parser *parser)
{
	if (line)
		free(line);
	if (token_list)
		free_token_list(token_list);
	if (parser)
		free_parser(parser);
}
