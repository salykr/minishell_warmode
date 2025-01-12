/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 10:41:09 by isalayan          #+#    #+#             */
/*   Updated: 2025/01/05 10:41:09 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./mini_shell.h"

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

void	print_2d_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i] != NULL)
	{
		printf("%d: %s\n", i, array[i]);
		i++;
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
		printf("Delimiter: \n");
		print_2d_array(curr->delimeter);
		printf("Infile:\n");
		print_2d_array(curr->infile);
		printf("Outfile:\n");
		print_2d_array(curr->outfile);
		printf("args:\n");
		print_2d_array(curr->args);
		printf("-----\n");
		curr = curr->next;
	}
}

// void print_tokens(const t_tokenlist *list)
// {
//     t_input *current = list->head;
//     while (current) 
//     {
//         const char *type_str;
//         switch (current->type) 
//         {
//             case T_NUMBER:            type_str = "NUMBER"; break;
//             case T_OPERATOR:          type_str = "OPERATOR"; break;
//             case T_PARENTHESIS:       type_str = "PARENTHESIS"; break;
//             case T_IDENTIFIER:        type_str = "IDENTIFIER"; break;
//             case T_WHITESPACE:        type_str = "WHITESPACE"; break;
//             case T_UNKNOWN:           type_str = "UNKNOWN"; break;
//             case T_ARGUMENT:          type_str = "ARGUMENT"; break;
//             case T_PIPE:              type_str = "PIPE"; break;
//             case T_REDIRECTION:       type_str = "REDIRECTION"; break;
//             case T_ENV:               type_str = "ENV"; break;
//             case T_BRACES:            type_str = "BRACES"; break;
//             case T_TILDE:             type_str = "TILDE"; break;
//             case T_QUOTE:             type_str = "QUOTE"; break;
//             case T_SEMICOLON:         type_str = "SEMICOLON"; break;
//             case T_AMPERSAND:         type_str = "AMPERSAND"; break;
//             case T_LOGICAL_AND:       type_str = "LOGICAL_AND"; break;
//             case T_LOGICAL_OR:        type_str = "LOGICAL_OR"; break;
//             case T_SUBSHELL:          type_str = "SUBSHELL"; break;
//             case T_PERIODS:           type_str = "PERIOD"; break;
//             case T_COMMAND_SEPARATOR: type_str = "COMMAND_SEPARATOR"; break;
//             case T_PATH:              type_str = "PATH"; break;
//             case T_HEREDOC:          type_str = "T_HEREDOC"; break;
//             case T_APPEND:          type_str = "T_APPEND"; break;
//             case T_INPUT:          type_str = "T_INPUT"; break;
//             case T_OUTPUT:          type_str = "T_OUTPUT"; break;
//             default:                  type_str = "INVALID"; break;
//         }   
//         printf("Token: %s, Value: %s\n", type_str, current->value);
//         current = current->next;
//     }
// }
