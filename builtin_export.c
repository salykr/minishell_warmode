/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:37:52 by skreik            #+#    #+#             */
/*   Updated: 2024/11/14 17:37:29 by marvin           ###   ########.fr       */
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
        }
        i++;  // Increment index manually
    }
}

/*

void builtin_export(t_parser *list, t_env *env)
{
    char *name;
    char *value;
    char *new_name;
    int i;
    int j;
    char *input_copy;
    char *first_quote;
    char *equals_sign;

    if (list->operations != NULL)
        return((void)printf("invalid option.\n"));
    if (!list->input || !list->input[0])
        return(print_env_sorted(env));
    j = -1;
    while (list->input[++j])
    {
        if(strchr(list->input[j],'\\'))
            list->input[j] = ft_escape_char(list->input[j]);  // Escape special characters
    }
    if (strchr(list->input[0], '$') != NULL && strchr(list->input[0], '=') == NULL)
    {
        input_copy= ft_strdup(list->input[0]);
        new_name = process_variable(input_copy, env);
        if (*new_name=='\0')           
            return(print_env_sorted(env),handle_memory_errors(input_copy, new_name));
        if (check_input(new_name)==0 || check_input(new_name)==-1)   //export $hey9hh   //يعاد النظر
            return(handle_memory_errors(input_copy, new_name));
        handle_memory_errors(input_copy, new_name);
    }
    i = 0;
    while (list->input[i])
    {
        first_quote = strchr(list->input[i], '"');  // Find the first quote
        if (first_quote)
        {
            equals_sign = strchr(first_quote + 1, '=');  // Find '=' after the first quote
            if (equals_sign && *(equals_sign + 1) == '"')
                list->input[i] = remove_closing_quote_after_equals(list->input[i]);
        }
        parse_export_input(list->input[i], &name, &value);  // Split input into name and value
        if (name!= NULL && check_input(name)==0)
        {
            handle_memory_errors(name, value);
            i++;
            continue;
        }
        if ((name!= NULL && strchr(name,';')) ||(value!= NULL && strchr(value,';')))
        {
            add_or_update_to_env(name, value, env);  // Update environment
            break;
        }
        if (name)
            add_or_update_to_env(name, value, env);  // Update environment
        i++;  // Increment index manually
    }
}
*/