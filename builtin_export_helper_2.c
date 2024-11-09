/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:57:01 by marvin            #+#    #+#             */
/*   Updated: 2024/10/18 13:57:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"

//removals
//remove backslashes
#include <stdlib.h>
#include <string.h>

static size_t count_backslashes(const char *str, size_t *index)
{
    size_t count = 0;

    while (str[*index] == '\\')
    {
        count++;
        (*index)++;
    }
    return count;
}

static void handle_backslashes(char *total, size_t *j, size_t backslash_count, char next_char)
{
    if (next_char == '$')
    {
        // Add all backslashes without any removal if next character is '$'
        while (backslash_count--)
            total[(*j)++] = '\\';
    }
    else if (backslash_count % 2 == 0)
    {
        // Add all backslashes if the count is even
        while (backslash_count--)
            total[(*j)++] = '\\';
    }
    else
    {
        // Add all but one backslash if the count is odd
        while (--backslash_count)
            total[(*j)++] = '\\';
    }
}

char *ft_escape_char(const char *str)
{
    char *total;
    size_t i = 0;
    size_t j = 0;
    size_t len = strlen(str);
    size_t backslash_count;

    total = malloc(len + 1);  // Allocate memory for the result string
    if (!total)
        return NULL;

    while (str[i] != '\0')
    {
        if (str[i] == '\\')
        {
            // Count consecutive backslashes
            backslash_count = count_backslashes(str, &i);

            // Determine the next character (or '\0' if at end)
            char next_char = str[i] ? str[i] : '\0';

            // Handle the backslashes based on their count and next character
            handle_backslashes(total, &j, backslash_count, next_char);
        }
        else
        {
            // Copy non-backslash characters directly
            total[j++] = str[i++];
        }
    }
    total[j] = '\0';  // Null-terminate the result
    return total;
}

char *ft_trim_string(char *str)
{
    char *trimmed_str = ft_strtrim(str, " ");  // Trim spaces from both sides
    if (trimmed_str)
    {
        free(str);  // Free the original string if dynamically allocated
        return trimmed_str;  // Return the newly trimmed string
    }
    return NULL;  // Return NULL if trimming failed
}

//ft_escape_char
/*
char *ft_escape_char(char *str)
{
    char *total;
    size_t i;
    size_t j;
    size_t len;
    size_t backslash_count;

    len = strlen(str);  // Length of the original string
    total = malloc(len + 1);  // Allocate memory for the result string
    i = 0;
    j = 0;
    if (!total)
        return (NULL);
        
    while (str[i] != '\0')
    {
        if (str[i] == '\\')
        {
            backslash_count = 0;
            while (str[i] == '\\')
            {
                backslash_count++;
                i++;
            }
            if (backslash_count % 2 == 0)
            {
                while (backslash_count--)
                    total[j++] = '\\';
            }
            else
            {
                while (--backslash_count)
                    total[j++] = '\\';
            }
        }
        else
            total[j++] = str[i++];
    }
    total[j] = '\0';  // Null-terminate the result
    return (total);
}
*/

//remove quotes 0
/*
void remove_edge_quotes(char *str)
{
    int len = strlen(str);

    // Check for starting quote
    if (len > 0 && (str[0] == '\'' || str[0] == '\"'))
    {
        // Shift the string left to remove the starting quote
        memmove(str, str + 1, len);
        len--;  // Update the length after removing the first character
    }

    // Check for ending quote
    if (len > 0 && (str[len - 1] == '\'' || str[len - 1] == '\"'))
    {
        str[len - 1] = '\0';  // Remove the ending quote by null-terminating
    }
}
*/

//remove quotes 1
/*
char *remove_quotes_new(const char *str)
{
    int i;
    int j;
    int len;
    char *new_str; 

    len = strlen(str);
    i = 0;
    j = 0;
    new_str = (char *)malloc(len + 1); // +1 for null terminator
    if (!new_str) // Check if memory allocation failed
        return (NULL);
    while (str[i] != '\0')
    {
        if (str[i] != '"' && str[i] != '\'')
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0'; // Null-terminate the new string
    return (new_str);
}
*/