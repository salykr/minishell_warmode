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

t_input* create_token(t_type type, const char *value)
{
    t_input *token;

    token = malloc(sizeof(t_input));
    if (!token)
        return NULL;
    token->value = ft_strdup(value);
    if (!token->value)
    {
        free(token);
        return NULL;
    }
    token->type = type;
    token->next = NULL;
    token->prev = NULL;

    return token;
}

t_tokenlist* create_token_list()
{
    t_tokenlist *list;

    list = malloc(sizeof(t_tokenlist));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void add_token(t_tokenlist *list, t_input *token)
{
    if (list->tail)
    {
        list->tail->next = token;
        token->prev = list->tail;
        list->tail = token;
    } 
    else
    {
        list->head = token;
        list->tail = token;
    }
}

void free_token(t_input *token)
{
  if (token)
    {
        free(token->value);
        free(token);
    }
}

void free_token_list(t_tokenlist *list)
{
    t_input *current;
    t_input *next;

    current = list->head;
    next = NULL;
    while (current)
    {
        next = current->next;
        free_token(current);
        current = next;
    }
    free(list);
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
