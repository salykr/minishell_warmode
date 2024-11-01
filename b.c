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
            char *end_of_var = strpbrk(var_name, " $?1234567890+'\"");
            char temp_char = '\0';
            char first_char = *var_name; // Save the first character of the variable name

            if (end_of_var)
            {
                temp_char = *end_of_var;
                *end_of_var = '\0';  // Temporarily terminate the variable name
            }

            // New condition: if the previous character is a single quote
            if (dollar > input && *(dollar - 1) == '\'')
            {
                // Find the closing quote and append the string as-is
                char *closing_quote = strchr(dollar + 1, '\'');
                if (closing_quote)
                {
                    strncat(new_str, dollar, closing_quote - dollar + 1);
                    total_size += (closing_quote - dollar + 1);
                    start = closing_quote + 1;
                }
                else
                    break;
            }
            // Check if the first character is '?'
            else if (first_char == '?')
            {
                char exit_status[4];
                snprintf(exit_status, sizeof(exit_status), "%d", global_var);

                total_size += strlen(exit_status);
                new_str = realloc(new_str, total_size);  // Resize the string to fit the new value
                if (!new_str)
                    return (NULL);
                strcat(new_str, exit_status);  // Concatenate the exit status
                     // Move start to the position after the variable
                if (end_of_var)
                    start = end_of_var + 1;  // Move past the variable
                else
                    break;  // No more characters after `$?`
            }
            else if (isdigit(first_char))
                start = var_name + 1;  // Skip the digit (which is just one character)
            else
            {
                char *env_value = get_env_value(env, var_name);
                if (env_value == NULL)
                    env_value = "";  // Treat it as an empty string
                total_size += strlen(env_value);
                new_str = realloc(new_str, total_size);  // Resize the string to fit the new value
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
        strcat(new_str, start);  // Append the remainder of the string
        
        return (remove_quotes(new_str));
    }
    return (remove_quotes(strdup(input)));  // Return a copy of input if no processing is needed
}
