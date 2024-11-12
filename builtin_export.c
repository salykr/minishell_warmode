/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:37:52 by skreik            #+#    #+#             */
/*   Updated: 2024/11/12 21:34:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"
//
//export hi=jey\$hi
//export hi=jey\\$hi
//i just have to edit the backslash f eza baada fe $ dont remove the $



void builtin_export(t_parser *list, t_env *env)
{
    char *name;
    char *value;
    char *new_name;
    int i;
    int j;

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
    while (list->input[j] )
    {
        if (strchr(list->input[j],'\\') && calculate_len_for_backslash(list->input[j]) != 0)
        {
            // char *s = list->input[j];
            // printf("s is: %s\n",s);
            list->input[j] = ft_escape_char(list->input[j]);  // Escape special characters
            // printf("new input is: %s\n",list->input[j]);
            // free(s);
        }
        j++;
    }
    if (strchr(list->input[0], '$') != NULL && strchr(list->input[0], '=') == NULL)
    {
        char *input_copy= ft_strdup(list->input[0]);
        new_name = process_variable(input_copy, env);
        if (*new_name=='\0')
        {
            print_env_sorted(env);  // Sorted environment display
            memory_free(input_copy, new_name);
            return;
        }
        if (check_input(new_name)==0 || check_input(new_name)==-1)
        {//export $hey9hh   //يعاد النظر
            memory_free(input_copy, new_name);
            return;
        }
        memory_free(input_copy, new_name);
    }
    i = 0;
    while (list->input[i])
    {
        //hello\there=hello\her
        list->input[i]=remove_closing_quote_after_equals(list->input[i]);
        parse_export_input(list->input[i], &name, &value);  // Split input into name and value
        if (name!= NULL && check_input(name)==0)
        {
            memory_free(name, value);
            i++;
            continue;
        }
        //value = ft_strtrim(value," ");
        if ((name!= NULL && strchr(name,';')) ||(value!= NULL && strchr(value,';')))
        {
            add_or_update_to_env(name, value, env);  // Update environment
            memory_free(name, value);
            break;
        }
        if (name)
        {
            add_or_update_to_env(name, value, env);  // Update environment
            memory_free(name, value);
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
