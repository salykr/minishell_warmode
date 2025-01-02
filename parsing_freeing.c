/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_freeing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:15:50 by marvin            #+#    #+#             */
/*   Updated: 2024/10/27 17:15:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	free_2d_array(char **array)
{
	char	**ptr;

	ptr = array;
	if (!array)
		return ;
	while (*ptr)
	{
		free(*ptr);
		ptr++;
	}
	free(array);
}

void	free_parser_helper(t_parser *parser)
{
	if (parser->outfile)
		free_2d_array(parser->outfile);
	if (parser->infile)
		free_2d_array(parser->infile);
	if (parser->args)
		free_2d_array(parser->args);
	if (parser->redirection)
		free(parser->redirection);
	if (parser->command)
		free(parser->command);
}

void	free_parser(t_parser *parser)
{
	t_parser	*next;

	while (parser)
	{
		next = parser->next;
		if (parser->input)
			free_2d_array(parser->input);
		if (parser->heredoc)
			free_2d_array(parser->heredoc);
		if (parser->delimeter)
			free_2d_array(parser->delimeter);
		if (parser->operations)
			free_2d_array(parser->operations);
		free_parser_helper(parser);
		free(parser);
		parser = next;
	}
}
