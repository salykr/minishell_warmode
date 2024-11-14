/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:37:52 by skreik            #+#    #+#             */
/*   Updated: 2024/11/13 15:18:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"
//
//export hi=jey\$hi
//export hi=jey\\$hi
//i just have to edit the backslash f eza baada fe $ dont remove the $

char *remove_closing_quote_after_equals(char *str)
{
    if (!str) return NULL;

    int len = strlen(str);
    char *result = (char *)malloc(len + 1); // Allocate memory for the result
    if (!result) return NULL;
    result[0] = '\0'; // Initialize result as an empty string

    int i = 0;
    while (str[i] != '\0') {
        // Look for the first single or double quote
        if (str[i] == '"' || str[i] == '\'') {
            char quote_type = str[i];
            int quote_start = i;
            i++; // Move past the opening quote

            // Look for the closing quote and check for the '=' in between
            int has_equals = 0;
            int quote_end = i;
            while (str[quote_end] != '\0' && str[quote_end] != quote_type) {
                if (str[quote_end] == '=') {
                    has_equals = 1;
                }
                quote_end++;
            }

            // If '=' is found between the quotes, do not copy the quotes themselves
            if (has_equals) {
                // Copy everything between the quotes except the quotes themselves
                strncat(result, str + i, quote_end - i);
            } else {
                // If no '=' found, keep the quotes and copy everything
                strncat(result, str + quote_start, quote_end - quote_start + 1);
            }

            i = quote_end + 1; // Move past the closing quote
        } else {
            // If not in a quoted section, copy characters as-is
            strncat(result, str + i, 1);
            i++;
        }
    }
    free(str);
    return result;
}

void builtin_export(t_parser *list, t_env *env)
{
    char *name;
    char *value;
    char *new_name;
    int i;
    int j;
    // char *s;

    if (list->operations != NULL)
    {
        printf("invalid option.\n");
        return;
    }
    if (!list->input || !list->input[0])
    {
        print_env_sorted(env);  // Sorted environment display
        return;
    }
    j = 0;
    while (list->input[j])
    {
        if(strchr(list->input[j],'\\'))
        {
            // s = list->input[j];
            list->input[j] = ft_escape_char(list->input[j]);  // Escape special characters
            // free(s);
        }
        j++;
    }
    if (strchr(list->input[0], '$') != NULL && strchr(list->input[0], '=') == NULL)
    {
        printf("in this condition~~~~~~~~``");
        char *input_copy= ft_strdup(list->input[0]);
        new_name = process_variable(input_copy, env);
        if (*new_name=='\0')
        {
            print_env_sorted(env);  // Sorted environment display
            handle_memory_errors(input_copy, new_name);
            return;
        }
        if (check_input(new_name)==0 || check_input(new_name)==-1)
        {//export $hey9hh   //يعاد النظر
           handle_memory_errors(input_copy, new_name);
            return;
        }
        handle_memory_errors(input_copy, new_name);
    }
    i = 0;
    while (list->input[i])
    {
        char *first_quote = strchr(list->input[i], '"');  // Find the first quote
        if (first_quote) {
            char *equals_sign = strchr(first_quote + 1, '=');  // Find '=' after the first quote
            if (equals_sign && *(equals_sign + 1) == '"') {
                list->input[i] = remove_closing_quote_after_equals(list->input[i]);
            }
        }
        // list->input[i]=remove_closing_quote_after_equals(list->input[i]);
        printf("the input: %s\n\n",list->input[i]);
        parse_export_input(list->input[i], &name, &value);  // Split input into name and value
        if (name!= NULL && check_input(name)==0)
        {
            handle_memory_errors(name, value);
            i++;
            continue;
        }
        // s = value;
        // value = ft_strtrim(value," ");
        // free(s);
        if ((name!= NULL && strchr(name,';')) ||(value!= NULL && strchr(value,';')))
        {
            add_or_update_to_env(name, value, env);  // Update environment
            // handle_memory_errors(name, value);
            break;
        }
        if (name)
        {
            add_or_update_to_env(name, value, env);  // Update environment
            // if (value != NULL)
            //     free(value);
            // if(name != NULL)
            //     free(name);
        }
        i++;  // Increment index manually
    }
}


// //export splitted:
// void clean_input(char **input)
// {
//     int j = 0;
//     while (input[j])
//     {
//         // input[j] = remove_quotes(input[j]);
//         char *s = input[j];
//         input[j] = ft_escape_char(input[j]);
//         free(s);
//         j++;
//     }
// }
// void handle_invalid_operations(t_parser *list)
// {
//     if (list->operations != NULL)
//     {
//         printf("invalid option.\n");
//         return;
//     }
// }
// char *process_variable_assignment(t_parser *list, t_env *env)
// {
//     if (strchr(list->input[0], '$') != NULL && strchr(list->input[0], '=') == NULL)
//     {
//         char *str = strdup(list->input[0]);
//         char *new_name = process_variable(str, env);
//         free(str);
//         return new_name;
//     }
//     return NULL;
// }
// void clean_input(char **input)
// {
//     int j = 0;
//     while (input[j])
//     {
//         input[j] = remove_quotes(input[j]);
//         char *s = input[j];
//         input[j] = ft_escape_char(input[j]);
//         free(s);
//         j++;
//     }
// }
// void parse_and_update_env(char **input, t_env *env)
// {
//     char *name, *value;
//     int i = 0;
//     while (input[i])
//     {
//         parse_export_input(input[i], &name, &value);
//         value = ft_trim_string(value);
//         if (name)
//         {
//             printf("BEFORE: name: %s\nvalue:%s\n", name, value);
//             add_or_update_to_env(name, value, env);
//             printf("AFTER: name: %s\nvalue:%s\n", name, value);
//             if (value) free(value);
//             free(name);
//         }
//         i++;
//     }
// }
// void builtin_export(t_parser *list, t_env *env)
// {
//     handle_invalid_operations(list);
//     if (!list->input || !list->input[0])
//     {
//         print_env_sorted(env);
//         return;
//     }

//     char *new_name = process_variable_assignment(list, env);
//     if (new_name && *new_name == '\0')
//     {
//         print_env_sorted(env);
//         free(new_name);
//         return;
//     }
//     free(new_name);

//     clean_input(list->input);
//     parse_and_update_env(list->input, env);
// }
