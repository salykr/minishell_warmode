/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:37:52 by skreik            #+#    #+#             */
/*   Updated: 2024/10/02 16:28:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"
char *ft_str_join(char *s1, char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    if (!result)
        return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
int compare(const void *a, const void *b) 
{
    return ft_strcmp(*(const char **)a, *(const char **)b);
}

void print_env_sorted(t_env *env)
{
    if (env == NULL || env->env == NULL)
    {
        fprintf(stderr, "Error: Environment is NULL\n");
        return;
    }
    // Get the number of environment variables
    int count = 0;
    while (env->env[count] != NULL)
        count++;
    // Create an array to track which variables have been printed
    int printed[count];
    int i = 0;
    while (i < count)
    {
        printed[i] = 0; // Initialize all to not printed
        i++;
    }
    int printed_count = 0;
    while (printed_count < count)
    {
        int min_idx = -1;
        int j = 0;

        while (j < count)
        {
            if (!printed[j] && (min_idx == -1 || strcmp(env->env[j], env->env[min_idx]) < 0))
            {
                min_idx = j; // Find the smallest unprinted variable
            }
            j++;
        }
        if (min_idx != -1)
        {
            // Find the position of the '='
            char *equal_sign = strchr(env->env[min_idx], '=');
            if (equal_sign)
            {
                // Print variable with value enclosed in quotes
                printf("declare -x %.*s=\"%s\"\n",
                       (int)(equal_sign - env->env[min_idx]),
                       env->env[min_idx],
                       equal_sign + 1);
            }
            else
            {
                // Print variable without '=' and quotes if no value is present
                printf("declare -x %s\n", env->env[min_idx]);
            }
            printed[min_idx] = 1; // Mark this variable as printed
            printed_count++; // Increment the count of printed variables
        }
    }
}


// Function to convert integer to string
char *itoa(int num) {
    char buffer[12]; // Buffer to hold the string representation of the integer
    snprintf(buffer, sizeof(buffer), "%d", num);
    return strdup(buffer); // Duplicate the string for return
}

char *get_env_value(t_env *env, const char *var) 
{
    // Check if var starts with '$' and skip it
    if (var[0] == '$') 
        var++; // Skip the '$' character

    // Special handling for $? which represents the last command exit status
    if (strcmp(var, "?") == 0) 
        return itoa(env->exit_code); // Convert exit_code to string

    int i = 0;
    char *entry;
    char *equals;

    // Iterate through the environment list
    while ((entry = env->env[i]) != NULL) 
    {
        equals = strchr(entry, '=');
        if (equals)
        {
            // Calculate the length of the variable name
            size_t var_len = equals - entry;
            // Check if the variable name matches
            if (strncmp(entry, var, var_len) == 0 && strlen(var) == var_len)
                return strdup(equals + 1); // Return the value after '='
        }
        i++;
    }
    // Variable not found, return empty string instead of NULL
    return strdup(""); 
}

int check_input(char *str)
{
    int i = 0;
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
        else if (ft_isalpha(str[i]) || isdigit(str[i]) || str[i]=='_' || str[i]=='\\' || str[i]=='$' 
            || (str[i - 1]=='$' && str[i + 1]=='?'))
            i++;
        else if (str[i]=='=')
            return (1);
        else
        {
            printf("Error: Invalid character '%c' in input.\n", str[i]);
            return(0);
        }
    }
    return (1);
}
int check_value(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == '(' || str[i] == ')' || str[i] == '&' || str[i] == '!' || str[i] == ';' )
        {
            printf("Error: Invalid character.\n");
            return(0);
        }
        else
            i++;
    }
    return (1);
}

void parse_export_input(char *input, char **name, char **value)
{
    char *equal_sign;

    equal_sign = ft_strchr(input, '=');
    *name = NULL;
    *value = NULL;
    if (check_input(input))
    {
        if (equal_sign)
        {
            *name = ft_strndup(input, equal_sign - input);
            *value = ft_strdup(equal_sign + 1);
        }
        else
        {
            *name = ft_strdup(input);
            *value = NULL;  // Set value to NULL if '=' is not present
        }
    }
}

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

int ft_haschar(char *str, char c)
{
    int i = 0;
    while(str[i])
    {
        if (str[i] == c)
            return 1;
        i++;
    }
    return 0;
}
char *ft_escape_char(char *str)
{
    char *total;
    size_t i;
    size_t j;
    size_t len;
    size_t backslash_count;

    len = strlen(str);  // Length of the original string
    total = malloc(len + 1);  // Allocate memory for the result string
    if (!total)
        return NULL;

    i = 0;
    j = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '\\')
        {
            // Count consecutive backslashes
            backslash_count = 0;
            while (str[i] == '\\')
            {
                backslash_count++;
                i++;
            }

            // If the count is even, copy all backslashes
            if (backslash_count % 2 == 0)
            {
                while (backslash_count--)
                    total[j++] = '\\';
            }
            // If the count is odd, copy backslash_count - 1 backslashes (i.e., remove one)
            else
            {
                while (--backslash_count)
                    total[j++] = '\\';
            }
        }
        else
        {
            // Copy non-backslash characters directly
            total[j++] = str[i++];
        }
    }
    total[j] = '\0';  // Null-terminate the result
    return total;
}

char *remove_quotes_new(const char *str)
{
    int i = 0, j = 0;
    int len = strlen(str);

    // Allocate memory for the new string (worst case: no quotes removed)
    char *new_str = (char *)malloc(len + 1); // +1 for null terminator

    if (!new_str) // Check if memory allocation failed
        return NULL;

    // Copy non-quote characters into the new string
    while (str[i] != '\0')
    {
        if (str[i] != '"')
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0'; // Null-terminate the new string

    return new_str;
}
// Example get_env_value function (needs to be properly implemented)
char *get_env_value(t_env *env, const char *name);
void add_or_update_to_env(char *name, char *value, t_env *env)
{
    int i = 0;
    int name_len;
    char *str = NULL;
    char *new_value = NULL;
    char *new_name = NULL;
     if (value != NULL)
    {
        if (!check_value(value))
            return; 
    }
    // Check for variables that reference other environment variables in the name
    if (name && strchr(name, '$') && !strstr(name, "\\$"))
    {
        new_name = malloc(strlen(name) + 1);  // +1 for null terminator
        if (!new_name)
            return;

        new_name[0] = '\0';  // Start with an empty string

        char *start = name;
        char *dollar = strchr(name, '$');

        while (dollar)
        {
            // Copy the part before $
            strncat(new_name, start, dollar - start);

            // Get the environment variable's name after $
            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " $");
            if (end_of_var)
            {
                *end_of_var = '\0';  // Temporarily end the string to isolate the variable name
                start = end_of_var;
            }
            else
            {
                start = var_name + strlen(var_name);
            }

            // Fetch the value of the environment variable
            str = get_env_value(env, var_name);
            if (str)
            {
                strcat(new_name, str);
            }

            // Move to the next part of the string
            dollar = strchr(start, '$');
        }

        // Copy any remaining part of the string after the last $
        strcat(new_name, start);

        // Replace name with new_name
        name = new_name;
    }

    name_len = strlen(name);

    // Check for variables that reference other environment variables in the value
    if (value!=NULL && strchr(value, '$') && !strstr(value, "\\$"))
    {
        new_value = malloc(strlen(value) + 1);  // +1 for null terminator
        if (!new_value)
            return;

        new_value[0] = '\0';  // Start with an empty string

        char *start = value;
        char *dollar = strchr(value, '$');

        while (dollar)
        {
            // Copy the part before $
            strncat(new_value, start, dollar - start);

            // Get the environment variable's name after $
            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " $");
            if (end_of_var)
            {
                *end_of_var = '\0';  // Temporarily end the string to isolate the variable name
                start = end_of_var;
            }
            else
            {
                start = var_name + strlen(var_name);
            }

            // Fetch the value of the environment variable
            str = get_env_value(env, var_name);
            if (str)
            {
                strcat(new_value, str);
            }

            // Move to the next part of the string
            dollar = strchr(start, '$');
        }

        // Copy any remaining part of the string after the last $
        strcat(new_value, start);

        // Replace value with new_value
        value = new_value;
    }

    if (ft_haschar(name,'\\'))
        name = ft_escape_char(name);
    if(value && ft_haschar(value, '\\'))
        value = ft_escape_char(value);

    // Check if the variable already exists
    while (env->env[i] != NULL)
    {
        if (strncmp(env->env[i], name, name_len) == 0 &&
            (env->env[i][name_len] == '=' || env->env[i][name_len] == '\0'))
        {
            // Variable found, update it
            free(env->env[i]);
            size_t new_len = name_len + (value ? strlen(value) : 0) + 1; // +1 for '=' or '\0'
            env->env[i] = malloc(new_len + 1);  // +1 for null terminator
            if (!env->env[i])
                return;
            if (value)
                snprintf(env->env[i], new_len + 1, "%s=%s", name, value);
            else
                snprintf(env->env[i], new_len + 1, "%s", name);
            free(new_value);  // Free the allocated new_value buffer
            free(new_name);   // Free the allocated new_name buffer
            return;
        }
        i++;
    }

    // Variable not found, add it
    char **new_env = realloc(env->env, sizeof(char *) * (i + 2));
    if (!new_env)
        return;
    env->env = new_env;
    size_t new_len = name_len + (value ? strlen(value) : 0) + 1; // +1 for '=' or '\0'
    env->env[i] = malloc(new_len + 1);  // +1 for null terminator
    if (!env->env[i])
        return;
    if (value)
        snprintf(env->env[i], new_len + 1, "%s=%s", name, value);
    else
        snprintf(env->env[i], new_len + 1, "%s", name);
    env->env[i + 1] = NULL;

    free(new_value);  // Free the allocated new_value buffer
    free(new_name);   // Free the allocated new_name buffer
}

void remove_edge_quotes(char *str)
{
    int len = strlen(str);

    // Check for starting quote
    if (len > 0 && (str[0] == '\'' || str[0] == '\"'))
    {
        // Shift the string left to remove the starting quote
        memmove(str, str + 1, len);
        len--;  // Update the length after removing the first character
    }

    // Check for ending quote
    if (len > 0 && (str[len - 1] == '\'' || str[len - 1] == '\"'))
    {
        str[len - 1] = '\0';  // Remove the ending quote by null-terminating
    }
}


void builtin_export(t_parser *list, t_env *env)
{
    char *name;
    char *value;
    if (list->operations != NULL)
    {
        write(1, "invalid option", 14);
        return;
    }
    if (!list->input || !list->input[0]) // If no arguments, print all environment variables
    {
        print_env_sorted(env);
        return;
    }
    int j = 0;
    while(list->input[j])
    {
        list->input[j]=remove_quotes_new(list->input[j]);
        
        j++;
    }
    //printf("no quotes: %s",list->input[0]);
    //handling "ls"=l
    for (int i = 0; list->input[i]; i++)
    {
        parse_export_input(list->input[i], &name, &value);
        if (name)
        {
            // printf("name: %s, value: %s\n",name, value);
            add_or_update_to_env(name, value, env);
            if(value)
                free(value);
            free(name);
        }
    }
}
