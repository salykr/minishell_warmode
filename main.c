/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:10:16 by skreik            #+#    #+#             */
/*   Updated: 2024/10/01 15:47:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int global_var = 0;  // Define the global variable

void	ft_free_env(t_env **myenv)
{
	int	i;

	i = 0;
	if (!myenv || !(*myenv))
		return ;
	while ((*myenv)->env[i])
	{
		free((*myenv)->env[i]);
		i++;
	}
	free((*myenv)->env);
	free(*myenv);
	*myenv = NULL;
}
void	print_2d_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i] != NULL)
	{
		printf("%s\n", array[i]);
		i++;
	}
}
void	print_token_list(t_tokenlist *list)
{
	t_input	*token;

	token = list->head;
	while (token)
	{
		printf("Type: %d, Value: %s\n", token->type, token->value);
		token = token->next;
	}
}

void	print_parser(t_parser *parser)
{
	t_parser	*curr;

	curr = parser;
	while (curr)
	{
		printf("Command: %s\n", curr->command ? curr->command : "None");
		printf("Input:\n");
		print_2d_array(curr->input);
		printf("Operations:\n");
		print_2d_array(curr->operations);
		printf("Redirections:\n");
		if (curr->redirection)
		{
			for (int i = 0; curr->redirection[i] != '\0'; i++)
			{
				printf("%d\n", curr->redirection[i]);
			}
		}
		printf("Delimiter: %s\n", curr->delimeter ? curr->delimeter : "None");
		printf("Infile: %s\n", curr->infile ? curr->infile : "None");
		printf("Outfile:\n");
		print_2d_array(curr->outfile);
		printf("-----\n");
		curr = curr->next;
	}
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
		// Free 2D arrays in parser
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
		free(parser); // Free the parser node itself
		parser = next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;
	t_env *myenv;
	t_tokenlist *token_list;
	t_parser *parser;
	int value ;
	myenv = init_env(envp);
	signal(SIGINT, ctrl_c_press);
	while (1)
	{
		line = readline("minishell>");
		if (!line) // If EOF is reached, break the loop
			break ;
		if (strlen(line) == 0)
        {
            free(line);
            continue;
        }
		if (*line)
			add_history(line);
		token_list = tokenize_input(line);
		if (!token_list)
		{
			fprintf(stderr,"Error: Failed to tokenize input.\n");
			free(line);
			continue ;
		}
		// Print tokens (for debugging)
		printf("\nTokens:______________________\n");
		print_token_list(token_list);
	
		// Parse tokens
		parser = create_parser(); // Initialize parser
		if (!parser)
		{
			fprintf(stderr, "Error: Failed to create parser.\n");
			free(line);
			free_token_list(token_list); // Free token list
			continue ;
		}

		value = parse_tokens(&parser, token_list, *myenv);
		// Check if parser is valid
		if (!parser || !parser->command || value == -1)
		{
			// fprintf(stderr, "Error: Invalid parser or command.\n");
			free(line);
			free_token_list(token_list); // Free token list
			free_parser(parser);         // Free parser
			continue ;
		}

		// Print parser nodes (for debugging)
		printf("\nParsed commands:\n");
		print_parser(parser);

		// Execute the parsed command
		cmds_exec(parser, myenv);

		// Cleanup allocated memory
		free(line);                  // Free the input line
		free_token_list(token_list); // Free token list
		free_parser(parser);         // Free parser
	}
	// Free the environment variables
	ft_free_env(&myenv);
	return (EXIT_SUCCESS);
}