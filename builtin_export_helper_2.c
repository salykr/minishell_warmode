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
int initialise_escape_char(size_t *i, size_t *j, char *str, char **total)
{
    *i = 0;
    *j = 0; 
    *total = malloc(strlen(str) + 1);  // Allocate memory for the result string
    if (!total)
        return 1;
    return 0;
}
char *ft_escape_char(char *str)
{
    char *total;
    size_t i;
    size_t j;
    size_t backslash_count;
    char next_char;
    
    if (initialise_escape_char(&i, &j, str, &total) == 1)
        return NULL;
    while (str[i] != '\0')
    {
        if (str[i] == '\\')
        {
            backslash_count = count_backslashes(str, &i);
            if (str[i]) 
                next_char = str[i];
            else
                next_char = '\0';
            handle_backslashes(total, &j, backslash_count, next_char);
        }
        else
            total[j++] = str[i++];
    }
    total[j] = '\0';  // Null-terminate the result
    return (free(str), total);
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

