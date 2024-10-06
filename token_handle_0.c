/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:12:39 by skreik            #+#    #+#             */
/*   Updated: 2024/09/11 13:22:00 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"


//tokenizing-------------------3
void handle_pipe(const char **p, t_tokenlist *token_list) 
{
    t_input *token = create_token(T_PIPE, "|");
    add_token(token_list, token);
    (*p)++;
}

//tokenizing-------------------6
void handle_redirection(const char **p, t_tokenlist *token_list)
{
    t_input *token;
    if (*(*p + 1) == **p) 
    {
        if (**p == '<')
            token = create_token(T_HEREDOC, "<<");
        else
            token = create_token(T_APPEND, ">>");
        (*p) += 2;
    }
    else 
    {
        if (**p == '<')
            token = create_token(T_INPUT, "<");
        else
            token = create_token(T_OUTPUT, ">");
        (*p)++;
    }
    add_token(token_list, token);
}
//tokenizing-------------------7
void handle_number(const char **p, t_tokenlist *token_list)
{
    const char *start = *p;
    while (!ft_isspace(**p) && **p!='\0')
        (*p)++;
    char *number_str = ft_strndup(start, *p - start);//implement strndup
    t_input *token = create_token(T_NUMBER, number_str);
    add_token(token_list, token);
    free(number_str);
}
//tokenizing-------------------8
void handle_argument(const char **p, t_tokenlist *token_list)
{
    const char *start = *p;
    (*p)++;
    while (!ft_isspace(**p) && **p!='\0')
        (*p)++;
    char *argument_str = ft_strndup(start, *p - start);
    t_input *token = create_token(T_ARGUMENT, argument_str);
    add_token(token_list, token);
    free(argument_str);
}
//tokenizing-------------------9
void handle_operator(const char **p, t_tokenlist *token_list)
{
    char op_str[2] = {**p, '\0'};
    t_input *token = create_token(T_OPERATOR, op_str);
    add_token(token_list, token);
    (*p)++;
}
//tokenizing-------------------10
void handle_path(const char **p, t_tokenlist *token_list)
{
    const char *start = *p;

    // Move pointer to process the path
    (*p)++;

    // Loop through the input until you reach a space or special character
    while (**p != '\0' && !ft_isspace(**p) && **p != '|' && **p != '>' && **p != '<' && 
           **p != '(' && **p != ')' && **p != '{' && **p != '}' && **p != '$' && 
           **p != '"' && **p != '\''  && **p != '+' && **p != '-' &&
           **p != '*')
    {
        (*p)++;
    }

    // Copy the path and create the token
    char *path_str = ft_strndup(start, *p - start);
    t_input *token = create_token(T_PATH, path_str);
    add_token(token_list, token);

    // Free the temporary path string
    free(path_str);
}


// command | grep 'pattern' > output.txt < input.txt -flag + - * / /usr/bin/path ~ ~/dir $HOME 'quoted string' \"double quoted string\" (parentheses) {braces} 123 456