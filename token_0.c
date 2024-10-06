/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:08:45 by skreik            #+#    #+#             */
/*   Updated: 2024/09/13 13:45:02 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "mini_shell.h"
// tokenizing-------------------4
// t_input* create_token(t_type type, const char *value)
// {
//     t_input *token = malloc(sizeof(t_input));
//     if (!token)
//         return NULL; // Handle memory allocation failure
//     token->type = type;
//     token->value = ft_strdup(value); // Allocate memory for the value
//     token->next = NULL; // Initialize next pointer
//     token->prev = NULL; // Initialize previous pointer
//     return (token);
// }

t_input* create_token(t_type type, const char *value)
{
    t_input *token = malloc(sizeof(t_input));
    if (!token)
        return NULL; // Handle memory allocation failure

    // Allocate memory for the value and assign it to the token's value field
    token->value = ft_strdup(value); 
    if (!token->value) // Check if ft_strdup failed
    {
        free(token); // Free the token structure to avoid memory leak
        return NULL;
    }

    token->type = type;
    token->next = NULL; // Initialize next pointer
    token->prev = NULL; // Initialize previous pointer

    return token;
}



// Create a new token list
//tokenizing-------------------2
t_tokenlist* create_token_list()
{
    t_tokenlist *list = malloc(sizeof(t_tokenlist));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

// Add a token to the list
//tokenizing-------------------5
void add_token(t_tokenlist *list, t_input *token)
{
    if (list->tail)
    {
        list->tail->next = token; // Link the current tail to the new token
        token->prev = list->tail; // Link the new token to the current tail
        list->tail = token; // Update the tail to the new token
    } 
    else
    {
        list->head = token; // The first token added is both head and tail
        list->tail = token;
    }
}



//tokenizing-------------------19
// Print all tokens
void print_tokens(const t_tokenlist *list)
{
    t_input *current = list->head;
    while (current) 
    {
        const char *type_str;
        switch (current->type) 
        {
            case T_NUMBER:            type_str = "NUMBER"; break;
            case T_OPERATOR:          type_str = "OPERATOR"; break;
            case T_PARENTHESIS:       type_str = "PARENTHESIS"; break;
            case T_IDENTIFIER:        type_str = "IDENTIFIER"; break;
            case T_WHITESPACE:        type_str = "WHITESPACE"; break;
            case T_UNKNOWN:           type_str = "UNKNOWN"; break;
            case T_ARGUMENT:          type_str = "ARGUMENT"; break;
            case T_PIPE:              type_str = "PIPE"; break;
            case T_REDIRECTION:       type_str = "REDIRECTION"; break;
            case T_ENV:               type_str = "ENV"; break;
            case T_BRACES:            type_str = "BRACES"; break;
            case T_TILDE:             type_str = "TILDE"; break;
            case T_QUOTE:             type_str = "QUOTE"; break;
            case T_SEMICOLON:         type_str = "SEMICOLON"; break;
            case T_AMPERSAND:         type_str = "AMPERSAND"; break;
            case T_LOGICAL_AND:       type_str = "LOGICAL_AND"; break;
            case T_LOGICAL_OR:        type_str = "LOGICAL_OR"; break;
            case T_SUBSHELL:          type_str = "SUBSHELL"; break;
            case T_PERIODS:           type_str = "PERIOD"; break;
            case T_COMMAND_SEPARATOR: type_str = "COMMAND_SEPARATOR"; break;
            case T_PATH:              type_str = "PATH"; break;
            case T_HEREDOC:          type_str = "T_HEREDOC"; break;
            case T_APPEND:          type_str = "T_APPEND"; break;
            case T_INPUT:          type_str = "T_INPUT"; break;
            case T_OUTPUT:          type_str = "T_OUTPUT"; break;
            default:                  type_str = "INVALID"; break;
        }   
        printf("Token: %s, Value: %s\n", type_str, current->value);
        current = current->next;
    }
}






// Free a token
void free_token(t_input *token)
{
  if (token)
    {
        free(token->value);
        free(token);
    }
}
// Free a token list
void free_token_list(t_tokenlist *list)
{
    t_input *current = list->head;
    while (current)
    {
        t_input *next = current->next;
        free_token(current);
        current = next;
    }
    free(list);
}
