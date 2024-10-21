/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_identifier                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:26:42 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 16:26:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include"mini_shell.h"


int is_all_spaces(const char *str)
{
    if (str == NULL)
        return 0;
    while (*str)
    {
        if (!ft_isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}

int handle_parsing_identifier_helper_errors_helper(t_input *tokens, t_parser *curr)
{
    if (curr->command == NULL && ft_strchr(tokens->value, '/'))
    {
        if (cmd_is_dir(tokens->value))
        {
            errmsg_cmd(tokens->value, NULL, "Is a directory");
            global_var = 0;
            return 1;
        }
        else
        {
            printf("bash: %s : No such file or directory\n", tokens->value);
            global_var = 127;
            return -1;
        }
    }
    else if (!check_balanced_quotes(tokens->value))
    {
        printf("Error: Unbalanced quotes in argument\n");
        global_var = 1;
        return -1;
    }
    return 0;
}







int handle_parsing_identifier_helper_errors(t_input *tokens, t_parser *curr)
{
    int value;

    if (!ft_strncmp(tokens->value, "\\n", 2))
    {
        if (curr->command == NULL)
        {
            printf("%s: command not found\n", (tokens->value) + 1);
            global_var = 127;
            return -1;
        }
    }
    else if (!ft_strcmp(tokens->value, ":") || !ft_strcmp(tokens->value, "!"))
    {
        global_var = 127;
        return -1;
    }
    else if (curr->command == NULL && !ft_strncmp(tokens->value, "&&", 2))
    {
        printf("bash: syntax error near unexpected token `&&' \n");
        global_var = 2;
        return -1;
    }
    value = handle_parsing_identifier_helper_errors_helper(tokens,curr);   
    return value;
}



int handle_parsing_identifier_helper(t_input *tokens, t_parser *curr,char *value)
{
    if (curr->command == NULL || !tokens->value[0])
        {
            if (is_all_spaces(tokens->value))
            {
                free(value);
                return -1;
            }
            printf("%s: command not found\n", value);
            global_var = 127;
            free(value);
            return -1;
        }
        else
            curr->input = add_string_to_2d_array(curr->input, tokens->value);
    return 0;
}




int handle_parsing_identifier(t_input *tokens, t_parser *curr, t_env env)
{
    char *value;
    char *temp_value;
    int return_value;

    if (tokens->type == T_IDENTIFIER)
    {
        return_value = handle_parsing_identifier_helper_errors(tokens,curr);
        if  (return_value == 1 || return_value == -1)
            return return_value;
        temp_value = remove_quotes(tokens->value);
        value = expand_variable(temp_value, env);
        free(temp_value);
        if (value == NULL || value[0] == '\0')
            return 1;
        else if ((is_executable(value) || ft_strncmp(value, "cd", 2) == 0 || 
                    ft_strncmp(value, "exit", 4) == 0 || ft_strncmp(value, "export", 6) == 0 || 
                    ft_strncmp(value, "unset", 5) == 0) && (curr->command == NULL && !is_all_spaces(value)))
                        curr->command = value;
        else if(handle_parsing_identifier_helper(tokens,curr,value) < 0)
            return -1;
        if (curr->command == NULL)
            free(value);
    }
    return 0;
}