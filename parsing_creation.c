/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_creation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:26:50 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 16:26:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"mini_shell.h"



t_parser *create_parser(void)
{
    t_parser *new_parser;

    new_parser = (t_parser *)malloc(sizeof(t_parser));
    if (new_parser == NULL)
        return (NULL);
    new_parser->command = NULL;
    new_parser->input = NULL;
    new_parser->args = NULL;
    new_parser->operations = NULL;
    new_parser->redirection = NULL;
    new_parser->delimeter = NULL;
    new_parser->prev = NULL;
    new_parser->next = NULL;
    new_parser->infile = NULL;
    new_parser->outfile = NULL;
    return (new_parser);
}

void add_parser_node(t_parser **head, t_parser *new_node)
{
    t_parser *current;

    if (head == NULL || new_node == NULL)
        return;
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = new_node;
    new_node->prev = current;
}

char *remove_quotes(const char *str)
{
    char *result;
    int j;
    int i;
    bool in_single = false;
    bool in_double = false;

    result = malloc(ft_strlen(str) + 1);
    j = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '"' && !in_single)
            in_double = !in_double;
        else
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    return result;
}

bool count_dash(char *str)
{
    int i = 0;
    int count =0;
    while (str[i])
    {
        if (str[i]== '-')
        {
            count++;
        }
        i++;
    }
    return (count == 1);    
}



void	free_parser(t_parser *parser)
{
	t_parser	*next;
	char		**input_array;
	char		**ops_array;
	char		**outfile_array;

	while (parser)
	{
		next = parser->next;
		if (parser->input)
		{
			input_array = parser->input;
			while (*input_array)
			{
				free(*input_array);
				input_array++;
			}
			free(parser->input);
		}
		if (parser->operations)
		{
			ops_array = parser->operations;
			while (*ops_array)
			{
				free(*ops_array);
				ops_array++;
			}
			free(parser->operations);
		}
		if (parser->outfile)
		{
			outfile_array = parser->outfile;
			while (*outfile_array)
			{
				free(*outfile_array);
				outfile_array++;
			}
			free(parser->outfile);
		}
		free(parser);
		parser = next;
	}
}