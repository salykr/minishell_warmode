/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:18:23 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 19:18:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"
char *process_variable(char *input, t_env *env)
{
    if (input == NULL)
        return NULL;

    if (strchr(input, '$'))
    {
        size_t total_size = 1;  // Start with 1 for null terminator
        char *start = input;
        char *dollar = strchr(input, '$');
        char *new_str = malloc(total_size);
        if (!new_str)
            return NULL;
        new_str[0] = '\0';  // Start with an empty string

        while (dollar)
        {
            // Count preceding backslashes before the dollar sign
            size_t backslash_count = 0;
            for (char *p = dollar - 1; p >= input && *p == '\\'; p--)
                backslash_count++;
            
            if (backslash_count % 2 != 0) // Odd number of backslashes
            {
                size_t part_length = dollar - start + 1;
                total_size += part_length;
                
                char *temp = realloc(new_str, total_size);
                if (!temp) {
                    free(new_str);
                    return NULL;
                }
                new_str = temp;

                strncat(new_str, start, part_length);
                start = dollar + 1;
                dollar = strchr(start, '$');
                continue;
            }

            size_t part_length = (start[dollar - start - 1] == '\'') ? dollar - start - 1 : dollar - start;
            total_size += part_length;

            char *temp = realloc(new_str, total_size);
            if (!temp) {
                free(new_str);
                return NULL;
            }
            new_str = temp;

            strncat(new_str, start, part_length);

            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " '$?1234567890+\"");
            char temp_char = '\0';
            char first_char = *var_name;

            if (end_of_var)
            {
                temp_char = *end_of_var;
                *end_of_var = '\0';
            }

            if (first_char == '?')
            {
                char exit_status[4];
                snprintf(exit_status, sizeof(exit_status), "%d", global_var);
                size_t status_len = strlen(exit_status);

                total_size += status_len;

                temp = realloc(new_str, total_size);
                if (!temp) {
                    free(new_str);
                    return NULL;
                }
                new_str = temp;

                strcat(new_str, exit_status);
                start = end_of_var ? end_of_var + 1 : var_name + 1;
            }
            else if (dollar > input && *(dollar - 1) == '\'' && temp_char == '\'')
            {
                size_t dollar_len = strlen(dollar);
                total_size += dollar_len;

                temp = realloc(new_str, total_size);
                if (!temp) {
                    free(new_str);
                    return NULL;
                }
                new_str = temp;

                strncat(new_str, dollar, dollar_len);
                start = end_of_var + 1;
                if (*start == '\0') break;
            }
            else if (isdigit(first_char))
            {
                start = var_name + 1;
            }
            else
            {
                char *env_value = get_env_value(env, var_name);
                if (!env_value) env_value = "";

                size_t env_len = strlen(env_value);
                total_size += env_len;

                temp = realloc(new_str, total_size);
                if (!temp) {
                    free(new_str);
                    return NULL;
                }
                new_str = temp;

                strcat(new_str, env_value);
                start = end_of_var ? end_of_var : var_name + strlen(var_name);
                free(env_value);
            }

            dollar = strchr(start, '$');
            if (dollar && *(dollar + 1) == '\0') break;
        }

        total_size += strlen(start) + 1;

        char *temp = realloc(new_str, total_size);
        if (!temp) {
            free(new_str);
            return NULL;
        }
        new_str = temp;

        strcat(new_str, start);
        printf("the new_str: %s\n\n", new_str);

        char *final_result = remove_paired_quotes(new_str);
        free(new_str);
        return final_result;
    }

    return remove_paired_quotes(input);
}

/*
    size_t add_length = dollar - start;
        total_size += add_length + 1;  // +1 for null terminator
        char *temp = realloc(new_str, total_size);
        if (!temp) {
            free(new_str);  // Free the original memory if realloc fails
            return NULL;     // Handle the error (return or other approach based on function logic)
        }
        new_str = temp;  // Assign temp to new_str if realloc is successful
        strncat(new_str, start, add_length);
*/

/*
char *process_variable(char *input, t_env *env)
{
    if (input == NULL)
        return (NULL);

    // Check if there is a dollar sign in the input
    if (strchr(input, '$'))
    {
        size_t total_size = 1;  // Start with 1 for null terminator
        char *start = input;
        char *dollar = strchr(input, '$');
        char *new_str = malloc(total_size);
        if (!new_str)
            return (NULL);
        new_str[0] = '\0';  // Start with an empty string

        while (dollar)
        {
            // Count preceding backslashes before the dollar sign
            size_t backslash_count = 0;
            for (char *p = dollar - 1; p >= input && *p == '\\'; p--)
                backslash_count++;
            // If odd number of backslashes, append dollar and skip expansion
            if (backslash_count % 2 != 0)
            {
                strncat(new_str, start, dollar - start + 1);
                total_size += (dollar - start + 1);
                new_str = realloc(new_str, total_size);
                if (!new_str)
                    return (NULL);
                start = dollar + 1;
                dollar = strchr(start, '$');
                continue;
            }
            // If even number of backslashes, process the dollar variable as usual
            // printf("dollar: %s\n\n",dollar);
            // printf("dollar-start: %c\n\n",start[dollar - start -1]);
            // if (start[dollar - start - 1] == '\'') 
            // {
            //     printf("hereeeeeeeeeeeeee\n\n");
            //     dollar--;
            // }
            // printf("dollar after: %s\n\n",dollar);
            if (start[dollar - start - 1] == '\'') 
                strncat(new_str, start, dollar - start - 1);
            else
                strncat(new_str, start, dollar - start);
            total_size += (dollar - start);
            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " '$?1234567890+\"");
            char temp_char = '\0';
            char first_char = *var_name;

            if (end_of_var)
            {
                temp_char = *end_of_var;
                *end_of_var = '\0';
            }
            if (first_char == '?')
            {
                char exit_status[4];
                snprintf(exit_status, sizeof(exit_status), "%d", global_var);
                total_size += strlen(exit_status);
                new_str = realloc(new_str, total_size);
                if (!new_str)
                    return (NULL);
                strcat(new_str, exit_status);
                if (end_of_var)
                    start = end_of_var + 1;
                else
                    break;
            }
            else if (dollar > input && *(dollar - 1) == '\'' && temp_char == '\'')
            {
                if (dollar)
                {
                    total_size += strlen(dollar);
                    new_str = realloc(new_str, total_size);
                    if (!new_str)
                        return (NULL);
                    strncat(new_str, dollar, strlen(dollar));
                    start = end_of_var + 1;
                    if (*start == '\0')
                        break;
                }
                else
                {
                    strcat(new_str, dollar);
                    break;
                }
            }
            else if (isdigit(first_char))
                start = var_name + 1;
            else
            {
                char *env_value = get_env_value(env, var_name);
                if (env_value == NULL)
                    env_value = "";
                total_size += strlen(env_value);
                new_str = realloc(new_str, total_size);
                if (!new_str)
                    return (NULL);
                strcat(new_str, env_value);
                if (end_of_var)
                {
                    *end_of_var = temp_char;
                    start = end_of_var;
                }
                else
                    start = var_name + strlen(var_name);
            }
            dollar = strchr(start, '$');
            if (dollar != NULL && *(dollar + 1) == '\0')
                break;
        }
        strcat(new_str, start);
        printf("the new_str: %s\n\n",new_str);
        return (remove_paired_quotes(new_str));
    }
    return (remove_paired_quotes(input));
}
*/
