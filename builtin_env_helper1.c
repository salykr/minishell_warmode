/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_helper1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:35:52 by marvin            #+#    #+#             */
/*   Updated: 2024/11/12 18:35:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char *check_env_input(t_parser *list)
{
    int i;

    if (list == NULL || list->input == NULL)
        return (NULL);
    i = 0;
    while (list->input[i] != NULL)
    {
        if (ft_strcmp(list->input[i], "env") == 0)
        {
            i++;
            continue;
        }
        if (ft_strchr(list->input[i], '=') != NULL)
            return (list->input[i]);
        return (list->input[i]);
    }
    return (NULL);
}


int check_ls_pwd_in_env(t_parser *list, t_env *myenv)
{
    int i;
  t_parser *parser;

    if (list == NULL || list->input == NULL)
        return 1;
    i = 0;
    while (list->input[i] != NULL)
        i++;
    if (i > 0)
    {
        parser = create_parser();
        if (ft_strcmp(list->input[i - 1], "pwd") == 0)
            parser->command = "pwd";
        else if (ft_strcmp(list->input[i - 1], "ls") == 0)
            parser->command = "ls";
        else
        {
            free(parser);
            return (1);
        }
        cmds_exec(parser, myenv,NULL);
        free(parser);
        return (0);
    }
    return (1);
}


int match_and_print_env(char *env_var, char **input_list)
{
    char *var_name;
    char *equal_sign;
    int j;

    j = 0;
    while (input_list[j] != NULL)
    {
        equal_sign = ft_strchr(input_list[j], '=');
        if (equal_sign != NULL)
        {
            var_name = ft_strndup(input_list[j], equal_sign - input_list[j]);
            if (ft_strncmp(env_var, var_name, equal_sign - input_list[j]) == 0 && env_var[equal_sign - input_list[j]] == '=')
            {
                printf("%s\n", input_list[j]);
                free(var_name);
                return (1);
            }
            free(var_name);
        }
        j++;
    }
    return (0);
}


void print_env_vars(t_env *myenv, char **input_list)
{
    int i;

    i = 0;
    while (myenv->env[i] != NULL)
    {
        if (!match_and_print_env(myenv->env[i], input_list))
            printf("%s\n", myenv->env[i]);
        i++;
    }
}

void print_all_env_vars(t_env *myenv)
{
    int i;

    i = 0;
    while (myenv->env[i] != NULL)
    {
        if(ft_strchr(myenv->env[i],'=')!=NULL)
            printf("%s\n", myenv->env[i]);
        i++;
    }
}
