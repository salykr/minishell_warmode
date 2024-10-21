/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:01:21 by saoun             #+#    #+#             */
/*   Updated: 2024/10/21 16:47:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"



int isempty(const char *str)
{
    return (str == NULL || str[0] == '\0');
}


char *expand_variable(const char *input ,t_env env)
{
    char    *dollar_sign;
    size_t  prefix_len;
    char    *prefix;
    char    *var_value;
    char    *result;

    dollar_sign = ft_strchr(input, '$');
    if (!dollar_sign)
        return ft_strdup(input);
    prefix_len = dollar_sign - input;
    prefix = ft_strndup(input, prefix_len);
    dollar_sign++;
    var_value = ft_getenv(&env,dollar_sign);
    if (var_value)
    {
        result = malloc(prefix_len + ft_strlen(var_value) + 1);
        if (result)
        {
           ft_strcpy(result, prefix);
           ft_strcat(result, var_value);
        }
    }
    else
    {
        result = malloc(prefix_len + ft_strlen(dollar_sign) + 1);
        if (result)
        {
            ft_strcpy(result, prefix);
            ft_strcat(result, dollar_sign);
        }
    }
    free(prefix);
    return result;
}




int parse_tokens_helper(t_input *tokens, t_parser *curr,t_env env)
{
    if (tokens->type == T_IDENTIFIER)
    {  
        if (handle_parsing_identifier(tokens, curr, env) < 0)
            return -1;
    }
    else if (tokens->type == T_PATH)
        return(handle_parsing_path(tokens, curr, env));
    else if (tokens->type == T_ARGUMENT)
    {
        if (handle_parsing_argument(tokens, curr) < 0)
            return -1;
    }
     else if (tokens->type == T_TILDE)
            curr->input = add_string_to_2d_array(curr->input, tokens->value);
    else if (tokens->type == T_ENV || tokens->type == T_NUMBER || tokens->type == T_PERIODS || tokens->type == T_QUOTE)
    {
        if (handle_parsing_quotes(tokens, curr,env) < 0)
            return -1;
    }
    else if (tokens->type == T_APPEND || tokens->type == T_HEREDOC || tokens->type == T_OUTPUT || tokens->type == T_INPUT)
    {
        if (handle_parsing_redirection(tokens, curr) < 0)
            return -1;
    }
    return 0;
}




int parse_tokens(t_parser **parser, t_tokenlist *list, t_env env)
{
    t_input *tokens;
    t_parser *curr;

    if (parser == NULL || *parser == NULL || list == NULL || list->head == NULL) 
        return(printf("Error: Null pointer passed to parse_tokens.\n"),-1);
    curr = *parser;
    tokens = list->head;
    while (tokens != NULL)
    {
        if (tokens->type == T_PIPE)
        {
            if(curr->command == NULL || tokens->next ==NULL)
            {
                global_var=2;
                return (printf("bash: syntax error near unexpected token `|' \n"),-1);
            }
            add_parser_node(parser, create_parser());
            curr = curr->next;
        }
        else if (parse_tokens_helper(tokens,curr,env)< 0)
           return -1;
        tokens = tokens->next;
    }
     return 0;
}
