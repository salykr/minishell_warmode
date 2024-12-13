/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:26:47 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 16:26:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int is_executable(char *cmd)
{
    char *path;
    char *path_copy;
    char *dir;
    char full_path[1024];

    path = getenv("PATH");
    if (path == NULL)
        return (0);
    path_copy = ft_strdup(path);
    dir = ft_strtok(path_copy, ":");
    while (dir != NULL)
    {
        ft_strcpy(full_path, dir);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (1);
        }
        dir = ft_strtok(NULL, ":");
    }
    free(path_copy);
    return (0);
}
int is_executable_PWD(t_env env, char *cmd)
{
    char *cmd_1;

    cmd_1 = get_path_PWD(env,cmd);    
    if (access(cmd_1, X_OK)==0)
    {
        free(cmd_1);
        return 1;
    }
    free(cmd_1);
    return 0;
}

int handle_parsing_path_helper_1(t_input *tokens, t_parser *curr,t_env env)
{

    if (curr->command == NULL && 
        (is_executable(tokens->value) || is_executable_PWD(env, tokens->value)))
    {
        curr->command = ft_strdup(tokens->value);
        if (curr->command == NULL)
        {
            perror("Failed to allocate memory");
            return -1;
        }
    }
    else if (curr->command != NULL && cmd_is_dir(tokens->value))
        curr->input = add_string_to_2d_array(curr->input, tokens->value);
    else
    {
        printf("bash: %s : No such file or directory\n", tokens->value);
        global_var = 127;
        return -1;
    }
    return 0;
}

int handle_parsing_path_helper_2(t_input *tokens, t_parser *curr)
{
    char *p;

    p = ft_strrchr(tokens->value, '/');
    if (p != NULL)
        p++;
    if (*p != '\0')
    {
        if (is_executable(p) || 
            ft_strncmp(p, "cd", 2) == 0 || ft_strncmp(p, "exit", 4) == 0 || 
            ft_strncmp(p, "export", 6) == 0 || 
            ft_strncmp(p, "unset", 5) == 0)
        {
            curr->command = ft_strdup(p);
            if (curr->command == NULL)
            {
                perror("Failed to allocate memory");
                return -1;
            }
        }
        else
            curr->input = add_string_to_2d_array(curr->input, tokens->value);
    } 
    else
        curr->input = add_string_to_2d_array(curr->input, tokens->value);
    return 0;
}


int handle_parsing_path(t_input *tokens, t_parser *curr, t_env env)
{
    if (tokens->type == T_PATH)
    {

        if (curr->command == NULL && cmd_is_dir(tokens->value))
        {
            errmsg_cmd(tokens->value, NULL, "Is a directory");
            global_var = 0;
            return 1;
        }  
        else if (!ft_strncmp(tokens->value, "./", 2))
        {
           if (handle_parsing_path_helper_1(tokens,curr,env) < 0)
                return -1;
        }
        else if( !ft_strncmp(tokens->value, "/", 1))
        {
             if (handle_parsing_path_helper_2(tokens,curr) < 0)
                return -1;
        }
        else if (!cmd_is_dir(tokens->value))
        {
            global_var = 127;
            return (printf("bash: %s : No such file or directory\n", tokens->value),-1);
        } 
    }
    return 0;
}
