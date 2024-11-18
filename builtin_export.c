/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:37:52 by skreik            #+#    #+#             */
/*   Updated: 2024/11/18 15:38:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"
//
//export hi=jey\$hi
//export hi=jey\\$hi
//i just have to edit the backslash f eza baada fe $ dont remove the $

void deal_with_esc_char(t_parser *list)
{
    int j;
    
    j = -1;
    while (list->input[++j])
    {
        if(strchr(list->input[j],'\\'))
            list->input[j] = ft_escape_char(list->input[j]);  // Escape special characters
    }
}

int deal_with_name_no_var(t_parser *list, t_env *env)
{
    char *input_copy;
    char *new_name;
    
    if (strchr(list->input[0], '$') != NULL && strchr(list->input[0], '=') == NULL)
    {
        input_copy= ft_strdup(list->input[0]);
        new_name = process_variable(input_copy, env);
        if (*new_name=='\0')
            return(print_env_sorted(env), handle_memory_errors(input_copy, new_name), 0);
        if (check_input(new_name)==0 || check_input(new_name)==-1)
            return(handle_memory_errors(input_copy, new_name), 1);//export $hey9hh   //يعاد النظر
        return(handle_memory_errors(input_copy, new_name), -1);
    }
    return(-1);
}
void deal_with_quotes(t_parser *list, int i)
{
    char *first_quote;
    char *equals_sign;

    first_quote = strchr(list->input[i], '"');  // Find the first quote
    if (first_quote)
    {
        equals_sign = strchr(first_quote + 1, '=');  // Find '=' after the first quote
        if (equals_sign && *(equals_sign + 1) == '"')
            list->input[i] = remove_closing_quote_after_equals(list->input[i]);
    }
}


// int builtin_export(t_parser *list, t_env *env)
// {
//     char *name;
//     char *value;
//     int i;
//     int return_value;

//     if (list->operations != NULL)
//         return((void)printf("invalid option.\n"),2);
//     if (!list->input || !list->input[0])
//         return(print_env_sorted(env),0);
//     deal_with_esc_char(list);
//     return_value = deal_with_name_no_var(list, env);
//     if(return_value != -1)
//         return(return_value);
//     i = 0;
//     return_value = 0;
//     while (list->input[i])
//     {
//         deal_with_quotes(list, i);
//         parse_export_input(list->input[i], &name, &value);  // Split input into name and value
//         if (name!= NULL && check_input(name)==0)
//         {
//             handle_memory_errors(name, value);
//             return_value = 1;
//             i++;
//             continue;
//         }
//         if ((name!= NULL && strchr(name,';')) ||(value!= NULL && strchr(value,';')))
//         {
//             return_value = add_or_update_to_env(name, value, env);  // Update environment
//             break;
//         }
//         if (name)
//             return_value = add_or_update_to_env(name, value, env);  // Update environment
//         i++;  // Increment index manually
//     }
//     return(return_value);
// }
int handle_export_input(char *input, t_env *env, int *return_value)
{
    char *name;
    char *value;

    parse_export_input(input, &name, &value);
    if (name && check_input(name) == 0)
    {
        handle_memory_errors(name, value);
        *return_value = 1;
        return 0;
    }
    if ((name && strchr(name, ';')) || (value && strchr(value, ';')))
    {
        *return_value=add_or_update_to_env(name, value, env);
        return(1);
    }
    if (name)
        *return_value = add_or_update_to_env(name, value, env);
    return 0;
}

int builtin_export(t_parser *list, t_env *env)
{
    int i;
    int return_value;

    i = 0;
    return_value = 0;
    if (list->operations != NULL)
        return ((void)printf("invalid option.\n"), 2);
    if (!list->input || !list->input[0])
        return (print_env_sorted(env), 0);
    deal_with_esc_char(list);
    if ((return_value = deal_with_name_no_var(list, env)) != -1)
        return return_value;
    while (list->input[i])
    {
        deal_with_quotes(list, i);
        if (handle_export_input(list->input[i++], env, &return_value))
            break;
    }
    return return_value;
}
