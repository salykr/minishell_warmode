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


// char *remove_paired_quotes(char *str)
// {
//     char quote_type;
//     int i;
//     int j;
//     int len;
//     char *result;
//     int start;

//     if (!str)
//         return NULL;
//     if (!strchr(str, '\'') && !strchr(str, '\"'))
//         return (str);
//     len = strlen(str);
//     result = (char *)malloc(len + 1); // Allocate memory for the result
//     if (!result)
//         return NULL;
//     result[0] = '\0';
//     i = 0;
//     j = 0;
//     while (str[i] != '\0')
//     {
//         if (str[i] == '"' || str[i] == '\'')
//         {
//             quote_type = str[i];
//             i++;
//             strncat(result, str + j, i - j - 1);
//             start = i;
//             while (str[i] != '\0' && str[i] != quote_type)
//                 i++;
//             if (str[i] == quote_type)
//             {
//                 strncat(result, str + start, i - start);
//                 i++;
//             }
//             j = i;
//         }
//         else
//             i++;
//     }
//     if (j < len) 
//         strncat(result, str + j, len - j);
//     return (free(str), result);
// }

int has_quotes(char *str)
{
    return strchr(str, '\'') || strchr(str, '\"');
}

//tosgheer
// void process_quotes(char *str, char *result, int *i, int *j, char quote_type)
// {
//     int start;

//     start = *i + 1;
//     *i += 1; // Skip opening quote
//     strncat(result, str + *j, *i - *j - 1); // Append before quote
//     while (str[*i] && str[*i] != quote_type) 
//         (*i)++;
//     if (str[*i] == quote_type)
//     { // Closing quote found
//         strncat(result, str + start, *i - start); // Append content within quotes
//         (*i)++; // Skip closing quote
//     }
//     *j = *i; // Update last processed position
// }

// char *remove_paired_quotes(char *str)
// {
//     int len;
//     char *result;
//     int i;
//     int j;

//     if (!str || !has_quotes(str)) 
//         return str;
//     len = strlen(str);
//     result = (char *)malloc(len + 1);
//     if (!result) 
//         return NULL;
//     result[0] = '\0';
//     i = 0;
//     j = 0;
//     while (str[i])
//     {
//         if (str[i] == '"' || str[i] == '\'')
//             process_quotes(str, result, &i, &j, str[i]);
//         else
//             i++;
//     }
//     if (j < len)
//         strncat(result, str + j, len - j);
//     return (free(str),result);
// }



//ij


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
    return (free(str),result);
}

// char *remove_closing_quote_after_equals(char *str)
// {
// 	int len;
// 	char *result;
// 	int i;
// 	char quote_type;
// 	int quote_start;
// 	int has_equals;
//     int quote_end;

//     if (!str) 
// 		return NULL;
//     result = (char *)malloc(len + 1); // Allocate memory for the result
// 	len = strlen(str);
//     if (!result) 
// 		return NULL;
//     result[0] = '\0'; // Initialize result as an empty string
//     i = 0;
//     while (str[i] != '\0')
// 	{
//         if (str[i] == '"' || str[i] == '\'')
// 		{
//             quote_type = str[i];
//             quote_start = i;
//             i++; // Move past the opening quote
//             has_equals = 0;
//             quote_end = i;
//             while (str[quote_end] != '\0' && str[quote_end] != quote_type)
// 			{
//                 if (str[quote_end] == '=')
//                     has_equals = 1;
//                 quote_end++;
//             }
//             if (has_equals)
//                 strncat(result, str + i, quote_end - i);
//             else
//                 strncat(result, str + quote_start, quote_end - quote_start + 1);
//             i = quote_end + 1; // Move past the closing quote
//         }
// 		else
// 		{
//             strncat(result, str + i, 1);
//             i++;
//         }
//     }
//     return (free(str),result);
// }


int has_equals_inside_quotes(char *str, int start, char quote_type)
{
    int i = start;
    int has_equals = 0;
    while (str[i] != '\0' && str[i] != quote_type)
	{
        if (str[i] == '=')
            has_equals = 1;
        i++;
    }
    return has_equals;
}

void copy_until_quote(char *str, char *result, int *i, int *j)
{
    strncat(result, str + *j, *i - *j); // Append non-quoted content
    *j = *i;
}

void fix_quote(char *str, char *result, int *i, int *j)
{
	char quote_type ;
	int quote_start ;
	int has_equals;
	int quote_end;

    quote_type = str[*i];
    quote_start = *i;
    (*i)++; // Move past the opening quote
    has_equals = has_equals_inside_quotes(str, *i, quote_type);
    quote_end = *i;
    while (str[quote_end] != '\0' && str[quote_end] != quote_type)
        quote_end++;
    if (has_equals)
        strncat(result, str + *i, quote_end - *i);
    else
        strncat(result, str + quote_start, quote_end - quote_start + 1);
    *i = quote_end + 1; // Move past the closing quote
    *j = *i; // Update position
}

char *remove_closing_quote_after_equals(char *str)
{
	int len;
	char *result;
	int i;
	int j;

    if (!str)
        return NULL;
    len = strlen(str);
    result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    result[0] = '\0';
    i = 0;
	j = 0;
    while (str[i] != '\0')
	{
        if (str[i] == '"' || str[i] == '\'')
            fix_quote(str, result, &i, &j);
        else
		{
            copy_until_quote(str, result, &i, &j);
            i++;
        }
    }
    return (free(str),result);
}
