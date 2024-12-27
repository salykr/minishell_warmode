/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_6.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:51:08 by marvin            #+#    #+#             */
/*   Updated: 2024/11/16 19:51:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int has_quotes(char *str)
{
    return strchr(str, '\'') || strchr(str, '\"');
}

void process_quotes(char *str, char *result, t_ij *indexes, char quote_type)
{
    int start;

    start = indexes->i + 1;
    indexes->i += 1; // Skip opening quote
    strncat(result, str + indexes->j, indexes->i - indexes->j - 1); // Append before quote
    while (str[indexes->i] && str[indexes->i] != quote_type) 
        indexes->i++;
    if (str[indexes->i] == quote_type)
    { // Closing quote found
        strncat(result, str + start, indexes->i - start); // Append content within quotes
        indexes->i++; // Skip closing quote
    }
    indexes->j = indexes->i; // Update last processed position
}

char *remove_paired_quotes(char *str)
{
    int len;
    char *result;
    t_ij indexes; // Initialize i and j

    indexes.i = 0;
    indexes.j = 0;
    if (!str || !has_quotes(str))
        return str;
    len = strlen(str);
    result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    result[0] = '\0';
    while (str[indexes.i])
    {
        if (str[indexes.i] == '"' || str[indexes.i] == '\'')
            process_quotes(str, result, &indexes, str[indexes.i]);
        else
            indexes.i++;
    }
    if (indexes.j < len)
        strncat(result, str + indexes.j, len - indexes.j);
    //printf("the name is:! %s\n",result);
    return (free(str),result);
}

