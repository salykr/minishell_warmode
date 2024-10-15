/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:37:52 by skreik            #+#    #+#             */
/*   Updated: 2024/10/15 17:24:21 by marvin           ###   ########.fr       */
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
char *itoa(int num) 
{
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
    return(NULL); 
}

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
    // if (check_input(input))
    // {
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
    // }
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
    i = 0;
    j = 0;
    if (!total)
        return (NULL);
        
    while (str[i] != '\0')
    {
        if (str[i] == '\\')
        {
            backslash_count = 0;
            while (str[i] == '\\')
            {
                backslash_count++;
                i++;
            }

            // If the number of backslashes is even, copy all of them
            if (backslash_count % 2 == 0)
            {
                while (backslash_count--)
                    total[j++] = '\\';
            }
            // If odd, copy all but one
            else
            {
                while (--backslash_count)
                    total[j++] = '\\';
            }
        }
        else
            total[j++] = str[i++];
    }
    total[j] = '\0';  // Null-terminate the result
        
    return (total);
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
        if (str[i] != '"' && str[i] != '\'')
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0'; // Null-terminate the new string

    return (new_str);
}

 char *process_variable(char *input, t_env *env)
{
    if (input == NULL)
        return (NULL);

    // Check if there is a dollar sign and it's not escaped
    if (strchr(input, '$') && !strstr(input, "\\$"))
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
            // Append the part of the string before the dollar sign
            strncat(new_str, start, dollar - start);
            total_size += (dollar - start);

            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " $?"); // Find the end of variable name
            char temp_char = '\0';
            char first_char = *var_name; // Save the first character of the variable name

            if (end_of_var)
            {
                temp_char = *end_of_var;
                *end_of_var = '\0';  // Temporarily terminate the variable name
            }

            // Check if the first character is '?'
            if (first_char == '?')
            {
                // Replace with the global exit status as a string
                char exit_status[4];  // Enough for numbers 0-255 (max 3 digits + null terminator)
                snprintf(exit_status, sizeof(exit_status), "%d", global_var);
                
                total_size += strlen(exit_status);
                new_str = realloc(new_str, total_size);  // Resize the string to fit the new value
                if (!new_str) return (NULL);
                strcat(new_str, exit_status);  // Concatenate the exit status
                printf("current new str %s\n\n",new_str);
                // Move start to the position after the variable
                if (end_of_var)
                {
                    start = end_of_var + 1;  // Move past the variable
                }
                else
                {
                    break;  // No more characters after `$?`
                }
            }
            else
            {
                // Retrieve the value from the environment
                char *env_value = get_env_value(env, var_name);
                
                // If the variable is not found, treat it as an empty string
                if (env_value == NULL)
                {
                    env_value = "";  // Treat it as empty string
                }

                total_size += strlen(env_value);  // Adjust total size for the env_value (even if empty)
                new_str = realloc(new_str, total_size);  // Resize the string to fit the new value
                if (!new_str) return (NULL);
                strcat(new_str, env_value);  // Concatenate environment variable value (or empty string)

                // Restore the character if it was saved
                if (end_of_var)
                {
                    *end_of_var = temp_char;  // Restore the character
                    start = end_of_var;  // Move start to the position after the variable
                }
                else
                {
                    start = var_name + strlen(var_name);  // Move past the variable name
                }
            }

            dollar = strchr(start, '$');  // Find the next dollar sign
            if (dollar != NULL)
            {
                char *dollar_1=dollar + 1;
                if (*dollar_1 == '\0')
                    break;
            }
        }

        strcat(new_str, start);  // Append the remainder of the string
        printf("new str %s\n",new_str);
        return (new_str);
    }

    return strdup(input);  // Return a copy of input if no processing is needed
}


void check_semicolon(char *name, char **value)
{
    // Find the first occurrence of ';' in the name
    char *semicolon_in_name = strchr(name, ';');
    if (semicolon_in_name != NULL)
    {
        // Nullify the character before ';' if it is '='
        if (semicolon_in_name != name && *(semicolon_in_name - 1) == '=')
        {
            *(semicolon_in_name - 1) = '\0';  // Remove the '=' character
        }
        *semicolon_in_name = '\0';  // Nullify everything after ';'
        
        // Safely nullify value if it's not NULL
        if (*value != NULL)
        {
            *value = NULL;  // Nullify the entire value
            return;
        }
    }
    else if (*value != NULL)  // Only check the value if it's non-NULL
    {
        // Find the first occurrence of ';' in the value
        char *semicolon_in_value = strchr(*value, ';');
        if (semicolon_in_value != NULL)
        {
            *semicolon_in_value = '\0';  // Nullify everything after ';' in the value
        }
    }
}


void add_or_update_to_env(char *name, char *value, t_env *env)
{
    int i = 0;
    int name_len;
    char *new_name = process_variable(name, env);
    char *new_value = value ? process_variable(value, env) : NULL;
    printf("new name : %s \nnew value: %s\n\n",new_name, new_value);
    if (!new_name || (value && !new_value))
    {
        free(new_name);
        free(new_value);
        return;  // Handle allocation error
    }
  check_semicolon(new_name, &new_value);

    int check_input_status = check_input(new_name);  // Call your check_input function
    if (check_input_status == 0 || (value != NULL && new_value != NULL && !check_value(new_value)))
    {
        free(new_name);
        free(new_value);
        return;
    }

    // If check_input_status == -1, remove the trailing '+' from new_name for concatenation
    if (check_input_status == -1)
    {
        size_t len = strlen(new_name);
        if (new_name[len - 1] == '+')
        {
            new_name[len - 1] = '\0';  // Remove the '+' for proper name matching
        }
    }

    name_len = strlen(new_name);

    while (env->env[i] != NULL)
    {
        if (strncmp(env->env[i], new_name, name_len) == 0 &&
            (env->env[i][name_len] == '=' || env->env[i][name_len] == '\0'))
        {
            if (check_input_status == -1)
            {
                // Append the new_value to the current value
                char *current_value = strchr(env->env[i], '=');
                if (current_value)
                {
                    current_value++;  // Move past the '='
                    size_t current_len = strlen(current_value);
                    size_t new_len = current_len + (new_value ? strlen(new_value) : 0);

                    // Allocate memory for the concatenated string
                    char *updated_value = malloc(new_len + 1);
                    if (!updated_value)
                    {
                        free(new_name);
                        free(new_value);
                        return;  // Memory allocation error
                    }

                    // Concatenate the current value and the new value
                    snprintf(updated_value, new_len + 1, "%s%s", current_value, new_value);

                    // Update the environment variable
                    free(env->env[i]);
                    env->env[i] = malloc(name_len + new_len + 2); // +2 for '=' and null terminator
                    if (!env->env[i])
                    {
                        free(new_name);
                        free(new_value);
                        free(updated_value);
                        return;  // Memory allocation error
                    }
                    snprintf(env->env[i], name_len + new_len + 2, "%s=%s", new_name, updated_value);

                    free(updated_value);
                    free(new_name);
                    free(new_value);
                    return;
                }
            }
            else
            {
                // Replace the value if check_input_status != -1
                free(env->env[i]);
                size_t new_len = name_len + (new_value ? strlen(new_value) : 0) + 1;
                env->env[i] = malloc(new_len + 1);
                if (!env->env[i])
                {
                    free(new_name);
                    free(new_value);
                    return;
                }
                if (new_value)
                    snprintf(env->env[i], new_len + 1, "%s=%s", new_name, new_value);
                else
                    snprintf(env->env[i], new_len + 1, "%s", new_name);

                free(new_name);
                free(new_value);
                return;
            }
        }
        i++;
    }

    // Variable not found, add it
    char **new_env = realloc(env->env, sizeof(char *) * (i + 2));
    if (!new_env)
    {
        free(new_name);
        free(new_value);
        return;
    }
    env->env = new_env;
    size_t new_len = name_len + (new_value ? strlen(new_value) : 0) + 1;
    env->env[i] = malloc(new_len + 1);
    if (!env->env[i])
    {
        free(new_name);
        free(new_value);
        return;
    }
    if (new_value)
        snprintf(env->env[i], new_len + 1, "%s=%s", new_name, new_value);
    else
        snprintf(env->env[i], new_len + 1, "%s", new_name);

    env->env[i + 1] = NULL;
    free(new_name);
    free(new_value);
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

void replace_with_str_NULL(char ***array, char *new_str)
{
    //same as replace_with_str, bas the only difference is that places an str incase it was passed
    size_t i;

    // Check if the array pointer itself is NULL
    if (!array)
        return;

    // Free the old array if it exists
    if (*array != NULL)
    {
        i = 0;
        while ((*array)[i] != NULL)
        {
            free((*array)[i]); // Free each string in the array
            i++;
        }
        free(*array); // Free the array itself
    }

    // Allocate memory for the new array
    *array = (char **)malloc(sizeof(char *) * 2);
    if (*array == NULL)
        return;

    // Set the first element to NULL if new_str is NULL
    if (new_str == NULL)
    {
        (*array)[0] = NULL; // Insert NULL into the array
    }
    else
    {
        (*array)[0] = strdup(new_str); // Duplicate the new string
        if ((*array)[0] == NULL)
        {
            free(*array); // Free the allocated memory in case of failure
            return;
        }
    }

    (*array)[1] = NULL; // Ensure the last element is NULL
}


void check_and_replace_env_value(t_parser *list, t_env *env)
{
    char *env_var_name;
    char *env_value;
    char global_var_str[4]; // Buffer to hold the string representation of global_var (max "256\0")
    if (list->input && list->input[0] && list->input[0][0] == '$')
    {
        env_var_name = list->input[0] + 1;
        env_value = ft_getenv(env, env_var_name);
        printf("env var name: %s\n",env_var_name);
        printf("val: %s\n",env_value);
        if (env_value != NULL || strcmp(env_var_name, "?") == 0)
        {
            if (strcmp(env_var_name, "?") == 0)
            {
                snprintf(global_var_str, sizeof(global_var_str), "%d", global_var);
                env_value = global_var_str; // Assign the string representation of the exit code
            }
            replace_with_str_NULL(&(list->input), env_value); // Replace the input with the value
        }
        else
            replace_with_str_NULL(&(list->input), NULL);
    }
}

char *ft_trim_string(char *str)
{
    char *trimmed_str = ft_strtrim(str, " ");  // Trim spaces from both sides
    if (trimmed_str)
    {
        free(str);  // Free the original string if dynamically allocated
        return trimmed_str;  // Return the newly trimmed string
    }
    return NULL;  // Return NULL if trimming failed
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

    // If no arguments are provided, print current environment variables
    if (!list->input || !list->input[0])
    {
        print_env_sorted(env);  // Sorted environment display
        return;
    }

    // Check if input contains an environment variable and no '='
    if (strchr(list->input[0], '$') != NULL && strchr(list->input[0], '=') == NULL)
    {
        check_and_replace_env_value(list, env);
         if (!list->input || !list->input[0])
        {
            print_env_sorted(env);  // Sorted environment display
            return;
        }
    }
    // Clean up and process each input entry
    int j = 0;
    while (list->input[j])
    {
        list->input[j] = remove_quotes(list->input[j]);  // Remove any quotes
        j++;
    }

    j = 0;
    char *s;
    while (list->input[j])
    {
        s = list->input[j];
        list->input[j] = ft_escape_char(list->input[j]);  // Escape special characters
        free(s);
        j++;
    }

    for (int i = 0; list->input[i]; i++)
    {
        parse_export_input(list->input[i], &name, &value);  // Split input into name and value

        if (name)
        {
            add_or_update_to_env(name, value, env);  // Update environment

            if (value)
                free(value);
            free(name);
        }
    }
}

//in one function, let's make :
//export $HOME
// and 
//export he$HOME=hi