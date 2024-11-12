/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:06:34 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 17:06:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"mini_shell.h"

int handle_parsing_quotes_helper_2(t_input *tokens,char * value)
{
    if (!strncmp(tokens->value,"$?",2))
    {
        if (!strcmp(tokens->value,"$?"))
        {
            printf("bash:%d: command not found\n", global_var);
            global_var=127;
            return -1; 
        }
        else
        {
            printf("bash:%d%d: command not found\n", global_var,global_var);
            global_var=127;
            return -1;
        }
    }
    else
    {
        printf("command %s not found\n",value);
        global_var=127;
        return -1;
    }
}

int handle_parsing_quotes_helper(t_input *tokens, t_parser *curr,char* value)
{
    if (curr->command!=NULL &&!ft_strcmp (curr->command,"echo")) //&& (!ft_strchr(tokens->value,'$')
        curr->input = add_string_to_2d_array(curr->input, tokens->value);
    else if (curr->command == NULL && cmd_is_dir(value))
    {
        errmsg_cmd(value, NULL, "Is a directory");
        free(value);
        global_var=0;
        return -1;
    }
    else if (curr->command == NULL)
    {
        handle_parsing_quotes_helper_2(tokens,value);
        free(value);
        return -1;
    }
    else
        curr->input = add_string_to_2d_array(curr->input, value);
    return 0;   
}



int handle_parsing_quotes(t_input *tokens, t_parser *curr,t_env env)
{
    char * value = NULL;
    char * temp_value= NULL;

    temp_value= remove_quotes(tokens->value);
    if (temp_value == NULL)
        return -1;
    value = expand_variable(temp_value, env);
    free(temp_value);  
    // if (value == NULL || value[0] == '\0')
    //     return 1;
    if ((tokens->value[0] == '\'' && tokens->value[1] == '\'' &&!tokens->value[2]) || (tokens->value[0] == '\"' && tokens->value[1] == '\"' && !tokens->value[2]))
            curr->input = add_string_to_2d_array(curr->input, tokens->value);
    else  if (curr->command == NULL &&(is_executable(value) || ft_strncmp(value, "cd", 2) == 0 || ft_strncmp(value, "exit", 4) == 0 || ft_strncmp(value, "export", 6) == 0 || ft_strncmp(value, "unset", 5) == 0))
    {
        curr->command = value;
        return 0;
    }
    else if (!strncmp(tokens->value,"\"-",2) || !strncmp(tokens->value,"\'-",2))
        curr->operations = add_string_to_2d_array(curr->operations, tokens->value);
    else if( handle_parsing_quotes_helper(tokens,curr,value) == -1)
        return -1;
    free(value);
    return 0;
}

int handle_parsing_redirection_helper(t_input *tokens, t_parser *curr)
{
    if (tokens->type == T_HEREDOC && (tokens->next->type == T_ENV || tokens->next->type == T_IDENTIFIER))
    {
        curr->delimeter = tokens->next->value;
        add_to_array(curr, 1);
    }
    else if (tokens->type == T_INPUT && tokens->next->type == T_IDENTIFIER)
    {
        curr->infile = tokens->next->value;
        add_to_array(curr, 3);
    }
    else if (tokens->type == T_OUTPUT && tokens->next->type == T_IDENTIFIER)
    {
        curr->outfile = add_string_to_2d_array(curr->outfile, tokens->next->value);
        add_to_array(curr, 2);
    }
    return 0;
}
int handle_parsing_redirection(t_input *tokens, t_parser *curr)
{
    //  if (curr->input == NULL  && curr->command == NULL)
    //         {
    //             printf("bash: syntax error near unexpected token `%s'\n", tokens->value);
    //             return -1;
    //         }
            if (tokens->next == NULL)
            {
                printf("bash: syntax error near unexpected token `newline`\n");
                global_var=2;
                return (-1);
            }
            else if (tokens->type == T_APPEND && tokens->next->type == T_IDENTIFIER)
            {
                curr->outfile = add_string_to_2d_array(curr->outfile, tokens->next->value);
                add_to_array(curr, 0);
            }
            else
                handle_parsing_redirection_helper(tokens,curr);
    return 0;
}
