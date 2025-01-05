/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:08:45 by skreik            #+#    #+#             */
/*   Updated: 2024/10/21 17:20:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "./mini_shell.h"

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


