/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitlin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:32:33 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 15:32:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"

void free_env(t_env *env)
{
    int i = 0;
    if (env)
    {
        while (env->env[i])
        {
            free(env->env[i]);
            i++;
        }
        free(env->env);
        free(env);
    }
}


void handle_memory_errors(char *new_name, char *new_value)
{
    free(new_name);
    free(new_value);
}


void free_name_and_value(char *new_name, char *new_value)
{
    free(new_name);
    free(new_value);
}
void free_2d_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i] != NULL)
    {
        free(array[i]);
        i++;
    }
    free(array);
}
void memory_free(char *str1, char *str2)
{
    if(str1 != NULL)
        free(str1);
    if (str2 != NULL)
        free(str2);
}

void free_memory(char *str1, char *str2, char **str3)
{
    if(str1 != NULL)
        free(str1);
    if (str2 != NULL)
        free(str2);
    if(str3 != NULL)
        free_2d_array(str3);
}

int ft_doublecharlen(t_env *env)
{
    int i;

    i = 0;
    while (env->env[i] != NULL)
        i++;
    return (i);
}

// Function to convert integer to string
char *itoa(int num) 
{
    char buffer[12]; // Buffer to hold the string representation of the integer
    snprintf(buffer, sizeof(buffer), "%d", num);
    return strdup(buffer); // Duplicate the string for return
}

int count_quote(const char *str)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(str[i]!='\0')
    {
        if (str[i]=='\'' || str[i]=='"')
            count++;
        i++;
    }
    return (count);
}
char *remove_quotes_new_new(const char *str)
{
    int i;
    int j;
    char *new_str;
    
    i = 0;
    j = 0;
    new_str = (char *)malloc(sizeof(char) * (strlen(str) - count_quote(str)) + 1);
    while(str[i] != '\0')
    {
        if (str[i]!='\'' && str[i]!='"')
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j]='\0';
    return(new_str);
}
char *remove_closing_quote_after_equals(const char *str)
{
    int len;
    int i;
    int quote_start;
    int has_equals;
    int quote_end;
    char *result;
    char quote_type;

    if (!str)
        return NULL;
    len = strlen(str);
    result = (char *)malloc(len + 1);
    if (!result)
        return (NULL);
    result[0] = '\0';
    i = 0;
    while (str[i] != '\0') 
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            quote_type = str[i];
            quote_start = i;
            i++;
            has_equals = 0;
            quote_end = i;
            while (str[quote_end] != '\0' && str[quote_end] != quote_type)
            {
                if (str[quote_end] == '=')
                    has_equals = 1;
                quote_end++;
            }
            if (has_equals)
                strncat(result, str + i, quote_end - i);
            else
                strncat(result, str + quote_start, quote_end - quote_start + 1);
            i = quote_end + 1; // Move past the closing quote
        }
        else 
        {
            strncat(result, str + i, 1);
            i++;
        }
    }
    printf("heeeeeeeeeeeeeeeeeeeeeeeeee\n");
    return (result);
}