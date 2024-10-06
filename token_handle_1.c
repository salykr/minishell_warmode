/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:14:53 by skreik            #+#    #+#             */
/*   Updated: 2024/10/03 14:53:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

//tokenizing-------------------11
void handle_tilde(const char **p, t_tokenlist *token_list)
{
    char tilde_str[3];
    if (*(*p + 1) && (*(*p + 1) == '-' || *(*p + 1) == '$' || *(*p + 1) == '/')) 
    {
        tilde_str[0] = **p;
        tilde_str[1] = *(*p + 1);
        tilde_str[2] = '\0';
        (*p)++;
    } 
    else 
    {
        tilde_str[0] = **p;
        tilde_str[1] = '\0';
    }
    t_input *token = create_token(T_TILDE, tilde_str);
    add_token(token_list, token);
    (*p)++;
}

//tokenizing-------------------13
void handle_env_variable(const char **p, t_tokenlist *token_list)
{
    const char *start = *p;
    if (**p!='\0')
        (*p)++;
    
    if (**p == '$' || **p == '?' || **p == '!' || **p == '@' || **p == '*' || **p == '_')
        {  
            if (**p!='\0')
                (*p)++;
        }
    else
    {
        while(!ft_isspace(**p) && **p!= '\0')
           (*p)++;
    }
    char *env_str = ft_strndup(start, *p - start);
    
    t_input *token = create_token(T_ENV, env_str);
    add_token(token_list, token);
    free(env_str);
}

//tokenizing-------------------14
void handle_parenthesis(const char **p, t_tokenlist *token_list)
{
    char paren_str[2] = {**p, '\0'};
    t_input *token = create_token(T_PARENTHESIS, paren_str);
    add_token(token_list, token);
    (*p)++;
}
//tokenizing-------------------15
// char *ft_find_str(const char *str, char c)
// {
//     const char *last_c = NULL;

//     while (*str && *str != '|')
//     {
//         if (*str == c)
//         {
//             last_c = str;
//         }
//         str++;
//     }
//     if (last_c && *(last_c +1)=='=')
//     {
//         while (*last_c!='\0' && *last_c!=' ')
//         {
//             last_c++;
//         }   
//     }
//     if (last_c == NULL)
//     {
//         last_c = str - 1;
//         while (last_c >= str && ft_isspace(*last_c))
//             last_c--;
//     }  
//     return ((char *)last_c); // Return the last occurrence found or the end of the string
// }

char *ft_find_str( const char **str, char c)
{
    const char *last_c = NULL;

    // Iterate through the string until null-terminator or '|'
    while (**str && **str != '|')
    {
        if ( **str==c )
        {
            while (**str && **str!= ' ')
            {
                (*str)++;   
            }
            last_c = *str -1;
            break; // Store the position of the last occurrence of 'c'
        }
        (*str)++; // Increment the main string pointer
        // printf("str: %c\n",**str);
    }
    // If 'c' was found and it's followed by '=', process further
    if (last_c && *(last_c + 1) == '=')
    {
        last_c++; // Move past the '='
        while (*last_c && *last_c != ' ')
        {
            last_c++; // Skip over characters until a space or end of string
        }
        while (*last_c != c && *last_c &&*last_c!=' ')
        {
            last_c++; // Move forward to find another occurrence of 'c'
        }
    }

    // If 'c' was never found, check for whitespaces before the current position
    if (!last_c)
    {
        last_c = *str - 1; // Start at the last position before null-terminator or '|'
        while (last_c >= *str && ft_isspace(*last_c))
        {
            last_c--; // Skip over any trailing whitespace
        }
    }

    return ((char *)last_c); // Return the position of the last occurrence or adjusted position
}

// void check_balanced_quotes1(t_quoted* quote_char,const char *input)
// {
//     char quote = input[0];  // No quote initially
//     int i = 1;
//     while (input[i])
//     {
//         if (input[i] == '\\')
//         {
//             if (input[i+1] != '\'' || input[i+1] != '\"')
//                 i++;
//         }
//         else if (input[i] == '\'' || input[i] == '\"')
//         {
//             if (input[i] == '\'' && quote_char->single == NULL && input[i] == quote)
//                 quote_char->single = input + i;
//             else if (input[i] == '\"' && quote_char->doubles == NULL)
//                 quote_char->doubles = input + i;
//             break;
//         }
//         i++;
//     }
// }
// tokenizing-------------------15
// void handle_quote(t_quoted* quote_char, const char **p, t_tokenlist *token_list)
// {
//     char quote_char;
//     const char *start;
//     char *end;
//     char *quoted_str;
//     t_input *token;
//     // if (**p == '\\') 
//     // {
//     //     start = *p;
//     //     (*p)++;
//     //     quote_char = **p; 
//     //     (*p)++;
//     // }
//     // else
//     // {
//     // check_balanced_quotes1(quote_char,*p);
//         quote_char = **p;
//         start = *p;
//         (*p)++;
//     // }
//     end = ft_find_str(p, quote_char);
//     quoted_str = ft_strndup(start, (end - start) + 1);
//     token = create_token(T_QUOTE, quoted_str);
//     add_token(token_list, token);
//     free(quoted_str);
//     *p = end +1;
// }


void handle_quote(const char **p, t_tokenlist *token_list)
{
    char quote_char;
    const char *start;
    char *end;
    char *quoted_str;
    t_input *token;
    if (**p == '\\' && **p!='\0') 
    {
        start = *p;
        (*p)++;
        quote_char = **p; 
        (*p)++;
    }
    else if(**p!='\0')
    {
        quote_char = **p;
        start = *p;
        (*p)++;
    }
    end = ft_find_str(p, quote_char);
    quoted_str = ft_strndup(start, (end - start) + 1);
    token = create_token(T_QUOTE, quoted_str);
    add_token(token_list, token);
    free(quoted_str);
    *p = end + 1;
}




// for test
// command | grep 'pattern' > output.txt < input.txt -flag + - * / /usr/bin/path ~ ~/dir $HOME 'quoted string' \"double quoted string\" (parentheses) {braces} 123 456