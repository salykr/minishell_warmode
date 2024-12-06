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

#include"mini_shell.h"


void free_input(char **input_array)
{
	char **ptr;

	ptr = input_array;
	if (!input_array)
		return;
	while (*ptr)
	{
		// printf("========freeing %s of size: %ld\n",*ptr,strlen(*ptr));
		free(*ptr);
		ptr++;
	}
	free(input_array);
}


void free_operations(char **ops_array)
{
	char	**ptr;

	ptr = ops_array;
	if (!ops_array)
        return;
	while (*ptr)
	{
		free(*ptr);
		ptr++;
	}
	free(ops_array);
}

void free_output(char **outfile_array)
{
	char	**ptr;

	ptr = outfile_array;
	if (!outfile_array)
        return;
	while (*ptr)
	{
		free(*ptr);
		ptr++;
	}
	free(outfile_array);
}


void free_parser(t_parser *parser)
{
	t_parser *next;

	while (parser)
	{
		// printf("freeing parser\n\n");
		next = parser->next;
		if (parser->input)
			free_input(parser->input);
		if (parser->delimeter)
			free(parser->delimeter);
		if (parser->operations)
			free_operations(parser->operations);
		if (parser->outfile)
			free_output(parser->outfile);
		if (parser->redirection)
			free(parser->redirection);
		if (parser->command)
			free(parser->command);
		free(parser);
		parser = next;
	}
}

