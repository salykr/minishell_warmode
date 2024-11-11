/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:51:31 by marvin            #+#    #+#             */
/*   Updated: 2024/10/18 13:51:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"
//checkers
void check_semicolon(char *name, char **value)
{
    char *semicolon_in_name;
    char *semicolon_in_value;

    semicolon_in_name = strchr(name, ';');
    if (semicolon_in_name != NULL)
    {
        if (semicolon_in_name != name && *(semicolon_in_name - 1) == '=')
            *(semicolon_in_name - 1) = '\0';
        *semicolon_in_name = '\0';
        if (*value != NULL)
        {
            *value = NULL;
            return;
        }
    }
    else if (*value != NULL)
    {
        semicolon_in_value = strchr(*value, ';');
        if (semicolon_in_value != NULL)
            *semicolon_in_value = '\0';
    }
}
int check_input(char *str)
{
    size_t i;

    i = 0;
    if (str[0] == '\0' || (!isalpha(str[0]) && (str[0] != '_'  && str[0]!='\'' && str[0]!='\"' && str[0]!='$')))
    {
        printf("Error: %c not a valid identifier.\n",str[0]);
        return (0);
    }
    while (str[++i])
    {
        if (str[i] == '=')
            return (1);
        if (!(ft_isalpha(str[i]) || isdigit(str[i]) || str[i] == '_' || str[i] == '?' 
            || str[i] == '\\' || str[i] == ';' || str[i] == '\'' || str[i] == '\"'|| str[i] == '$') 
            || (str[i] == '\\' && str[i + 1] == '\\'))
        {
            if((i == ft_strlen(str) - 1) && str[ft_strlen(str) - 1] == '+')
                    return (-1);
            printf("Error: %c not a valid identifier.\n",str[i]);
            return 0;
        }
    }
    return (1);
}
int check_input_end(char *str)
{
    size_t i;
    i = 0;
    if (str[0] == '\0' || (!isalpha(str[0]) && str[0] != '_'))
    {
        printf("Errorrr: %c not a valid identifier.\n",str[0]);
        return (0);
    }
    while (str[++i])
    {
        if (!(ft_isalpha(str[i]) || isdigit(str[i]) || str[i] == '_' || str[i] == '\\' || str[i] == ';') 
            || (str[i] == '\\' && str[i + 1] == '\\'))
        {
            if((i == ft_strlen(str) - 1) && str[ft_strlen(str) - 1] == '+')
                return -1;
            printf("Errorrr: %c not a valid identifier.\n",str[i]);
            return (0);
        }
    }
    return (1);
}
/*
int check_input(char *str)
{
    int i = 0;
    if(str[0]=='\0')
    {
        printf("Error.\n");
        return(0);
    }
    if (str[ft_strlen(str)-1]=='+')
        return(-1);
    if (!isalpha(str[0]) && str[0] != '_')
    {
        printf("Error: Invalid character.\n");
        return(0);
    }
    i++;
    while (str[i])
    {
        if (str[i]=='\\' && str[i + 1]=='\\')
        {
            printf("Error.\n");
            return(0);
        }
        else if (ft_isalpha(str[i]) || isdigit(str[i]) || str[i]=='_' || str[i]=='\\' || str[i] == ';')
            i++;
        else if (str[i]=='=')
            return (1);
        else
        {
            printf("Error.\n", str[i]);
            return(0);
        }
    }
    return (1);
}
*/
int check_value(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '(' || str[i] == ')' || str[i] == '&' || str[i] == '!' || str[i] == ';' )
        {
            printf("Error.\n");
            return(0);
        }
        else
            i++;
    }
    return (1);
}

int ft_haschar(char *str, char c)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}
