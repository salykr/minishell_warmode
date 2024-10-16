char *process_variable(char *input, t_env *env)
{
    if (input == NULL)
        return (NULL);

    // Check if there is a dollar sign and it's not escaped
    if (strchr(input, '$') && !strstr(input, "\\$"))
    {
        size_t total_size = strlen(input) + 1;  // Start with the size of the input plus null terminator
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

            char *next_char = dollar + 1;
            if (isdigit(*next_char))  // Check if the character after $ is a digit
            {
                // Skip the dollar sign and the single digit
                start = next_char + 1;
            }
            else
            {
                // If not a digit, just append the dollar sign and continue
                strncat(new_str, dollar, 1);
                start = dollar + 1;
            }

            dollar = strchr(start, '$');  // Find the next dollar sign
        }

        strcat(new_str, start);  // Append the remainder of the string
        return (new_str);
    }

    return strdup(input);  // Return a copy of input if no processing is needed
}
