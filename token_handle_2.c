/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:16:23 by skreik            #+#    #+#             */
/*   Updated: 2024/09/20 13:23:40 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"
//tokenizing-------------------16
void handle_braces(const char **p, t_tokenlist *token_list)
{
    char brace_str[2] = {**p, '\0'};
    t_input *token = create_token(T_BRACES, brace_str);
    add_token(token_list, token);
    (*p)++;
}

void handle_quote1(const char **p)
{
    char quote_char;
    char *end;
    if (**p == '\\') 
    {
        (*p)++;
        quote_char = **p; 
        (*p)++;
    }
    else
    {
        quote_char = **p;
        (*p)++;
    }
    end = ft_find_str(p, quote_char);
    *p = end +1;
}

//tokenizing-------------------17
void handle_identifier(const char **p, t_tokenlist *token_list)
{
    const char *start = *p;
    while (!ft_isspace(**p) && **p != '\0' && **p!= '>' && **p != '<' && ft_strncmp(*p, ">>", 2) && ft_strncmp(*p,"<<", 2) && **p!='|')
    {
        if (**p == '"' || **p == '\'' || (**p == '\\' && (**(p+1) == '"' ||**(p+1) == '\'')))
            {
                handle_quote1(p);
            }
        (*p)++;
    }
    char *identifier_str = ft_strndup(start, *p - start);
    t_input *token = create_token(T_IDENTIFIER, identifier_str);
    add_token(token_list, token);
    free(identifier_str);
}
//tokenizing-------------------12
// void handle_period(const char **p, t_tokenlist *token_list)
// {
//     char period_str[3];
//     period_str[0] = **p;
//     //printf("\n char is: %c and %c\n",**p,*(*p + 1));
//     if (*(*p + 1) && (*(*p + 1) == '.' || *(*p + 1) == '/'))
//     {
//         period_str[1] = *(*p + 1);
//         period_str[2] = '\0';
//         (*p)++;
//     } 
//     else
//         period_str[1] = '\0';
//     t_input *token = create_token(T_PERIODS, period_str);
//     add_token(token_list, token);
//     (*p)++;
// }

//tokenizing-------------------18
void handle_unknown(const char **p, t_tokenlist *token_list)
{
    char unknown_str[2] = {**p, '\0'};
    t_input *token = create_token(T_UNKNOWN, unknown_str);
    add_token(token_list, token);
    (*p)++;
}

//tokenizing-------------------1
t_tokenlist * tokenize_input(const char *input)
{
    const char *p = input;
    t_tokenlist *token_list = create_token_list();

    while (*p != '\0') 
    {
        while (ft_isspace(*p))
            p++;
        if(((*p == '\'' && *(p + 1) == '\'') || (*p == '\"' && *(p + 1) == '\"')) && ft_isalpha(*(p+2)))
        {
            p += 2;
        }
        if (*p == '|')
        {
            handle_pipe(&p, token_list);
            continue;
        }
        else if (*p == '<' || *p == '>')
        {
            handle_redirection(&p, token_list);
            continue;
        }
        else if (ft_isdigit(*p))
        {
            handle_number(&p, token_list);
            continue;
        }
        else if ((*p == '-' && ft_isalpha(*(p + 1))) || ((*p == '-') && (*(p+1) == '-') && (ft_isalpha(*(p + 2)))))
        {
            handle_argument(&p, token_list);
            continue;
        }
        // else if ((*p == '+' || *p == '-' || *p == '*' || *p == '/') && !ft_isalpha(*(p + 1)))
        // {
        //     handle_operator(&p, token_list);
        //     continue;
        // }
        else if ((*p == '/' && ft_isalpha(*(p + 1))) || (*p == '.' && *(p+1) == '/'))
        {
            handle_path(&p, token_list);
            continue;
        }
        else if((*p == '~' && *(p+1) == '/') && ft_isalpha(*(p + 2)))
        {
            handle_path(&p, token_list);
            continue;
        }
        else if (*p == '~')
        {
            handle_tilde(&p, token_list);
            continue;
        }
        // else if (*p == '.')
        // {
        //     handle_period(&p, token_list);
        //     continue;
        // }
        else if (*p == '$')
        {
            handle_env_variable(&p, token_list);
            continue;
        }
        else if (*p == '(' || *p == ')')
        {
            handle_parenthesis(&p, token_list);
            continue;
        }
        else if (*p == '"' || *p == '\'' || (*p == '\\' && (*(p+1) == '"' ||*(p+1) == '\'')))
        {
            handle_quote(&p, token_list);
            continue;
        }
        else if (*p == '{' || *p == '}')
        {
            handle_braces(&p, token_list);
            continue;
        }
        // else if (ft_isalpha(*p))
        else if (*p != '\0')
        {
            handle_identifier(&p, token_list);
            continue;
        }
        // else if (*p != '\0' && !ft_isspace(*p))
        // {
        //     handle_unknown(&p, token_list);
        //     continue;
        // }
    }
    print_tokens(token_list);
    return(token_list);
}
