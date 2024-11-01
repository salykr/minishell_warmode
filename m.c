//asghar she
/*
int find_and_update_env(int check_input_status, char *new_name, char *new_value, t_env *env)
{
    int i;
    size_t name_len;

    i = 0;
    name_len = strlen(new_name);
    while (env->env[i] != NULL)
    {
        if (strncmp(env->env[i], new_name, strlen(new_name)) == 0 &&
            (env->env[i][strlen(new_name)] == '=' || env->env[i][strlen(new_name)] == '\0'))
            {
                if (check_input_status == -1)
                {
                    char *current_value = strchr(env->env[i], '=');
                    if (current_value) {
                        current_value++;  // Move past the '='
                        char *updated_value = concatenate_value(current_value, new_value);
                        if (updated_value)
                        {
                            free(env->env[i]);
                            env->env[i] = malloc(strlen(new_name) + strlen(updated_value) + 2);
                            if (!env->env[i])
                            {
                                free_name_and_value(new_name, new_value);
                                free(updated_value);
                                return (1);  // Memory allocation error
                            }
                            strcpy(env->env[i], new_name);
                            strcat(env->env[i], "=");
                            strcat(env->env[i], updated_value);
                            free(updated_value);
                        }
                    }
                    else
                    {
                        size_t new_len = name_len + (new_value ? strlen(new_value) : 0) + 1;
                        env->env[i] = malloc(new_len + 2);
                        if (!env->env[i])
                        {
                            free_name_and_value(new_name, new_value);
                            return (1);  // Memory allocation error
                        }
                        if (new_value)
                            snprintf(env->env[i], new_len + 2, "%s=%s", new_name, new_value);
                        else
                            snprintf(env->env[i], new_len + 2, "%s=", new_name);
                        free_name_and_value(new_name, new_value);
                        return (1);  // Found and updated, return 1
                    }
                }
                else
                    replace_or_append_value(&env->env[i], new_name, new_value);
                handle_memory_errors(new_name, new_value);
                return (1);  // Found and updated, return 1
        }
        i++;
    }
    return (0);
}
void replace_or_append_value(char **env_entry, char *new_name, char *new_value)
{
    size_t name_len;
    size_t new_len;

    name_len = strlen(new_name);
    new_len = name_len + (new_value ? strlen(new_value) : 0) + 1;
    *env_entry = malloc(new_len + 1);
    if (!*env_entry)
        return;  // Memory allocation error
    ft_strcpy(*env_entry, new_name);
    if (new_value)
    {
        ft_strcat(*env_entry, "=");
        ft_strcat(*env_entry, new_value);
    }
    else
        ft_strcat(*env_entry, "");
}
void add_or_update_to_env(char *name, char *value, t_env *env)
{
    char *new_name;
    char *new_value;
    int i;
    int check_input_status;
    size_t len;
    char **new_env;

    new_name = process_variable(name, env);    
    if (value != NULL)
        new_value = process_variable(value, env);
    else
        new_value = NULL;
    printf("AFTER: new_name: %s\nnew_value: %s\n\n",new_name, new_value);

    if (!new_name || (value && !new_value))
    {
        handle_memory_errors(new_name, new_value);
        return;  // Handle allocation error
    }
    check_semicolon(new_name, &new_value);
    check_input_status = check_input(new_name);
    if (check_input_status == 0 || (value != NULL && new_value != NULL && !check_value(new_value)))
    {
        handle_memory_errors(new_name, new_value);
        return;
    }
    if (check_input_status == -1)
    {
        len = strlen(new_name);
        if (new_name[len - 1] == '+')
            new_name[len - 1] = '\0';  // Remove the '+' for proper name matching
    }
    if (find_and_update_env(check_input_status, new_name, new_value, env))
        return;
    i = ft_doublecharlen(env);
    new_env = realloc(env->env, sizeof(char *) * (i + 2));
    if (!new_env)
    {
        handle_memory_errors(new_name, new_value);
        return;
    }
    env->env = new_env;
    replace_or_append_value(&env->env[i], new_name, new_value);
    env->env[i + 1] = NULL;
    handle_memory_errors(new_name, new_value);
}
*/

//idk wad3a
/*
void add_or_update_to_env(char *name, char *value, t_env *env)
{
    char *new_name;
    char *new_value;
    int i;
    int check_input_status;
    size_t len;
    char **new_env;

    new_name = process_variable(name, env);    
    if (value != NULL)
        new_value = process_variable(value, env);
    else
        new_value = NULL;
    printf("AFTER: new_name: %s\nnew_value: %s\n\n",new_name, new_value);

    if (!new_name || (value && !new_value))
    {
        handle_memory_errors(new_name, new_value);
        return;  // Handle allocation error
    }
    check_semicolon(new_name, &new_value);
    check_input_status = check_input(new_name);
    if (check_input_status == 0 || (value != NULL && new_value != NULL && !check_value(new_value)))
    {
        handle_memory_errors(new_name, new_value);
        return;
    }
    if (check_input_status == -1)
    {
        len = strlen(new_name);
        if (new_name[len - 1] == '+')
            new_name[len - 1] = '\0';  // Remove the '+' for proper name matching
    }
    if (find_and_update_env(check_input_status, new_name, new_value, env))
        return;
    i = ft_doublecharlen(env);
    new_env = realloc(env->env, sizeof(char *) * (i + 2));
    if (!new_env)
    {
        handle_memory_errors(new_name, new_value);
        return;
    }
    env->env = new_env;
    replace_or_append_value(&env->env[i], new_name, new_value);
    env->env[i + 1] = NULL;
    handle_memory_errors(new_name, new_value);
}
*/

//medium size
/*

void update_env_variable(char *new_name, char *new_value, char **env_entry, int check_input_status) {
    if (check_input_status == -1) {
        char *current_value = strchr(*env_entry, '=');
        if (current_value) {
            current_value++;  // Move past the '='
            char *updated_value = concatenate_value(current_value, new_value);
            if (updated_value) {
                free(*env_entry);
                *env_entry = malloc(strlen(new_name) + strlen(updated_value) + 2);
                if (!*env_entry) {
                    free(new_name);
                    free(new_value);
                    free(updated_value);
                    return;  // Memory allocation error
                }
                strcpy(*env_entry, new_name);
                strcat(*env_entry, "=");
                strcat(*env_entry, updated_value);
                free(updated_value);
            }
        }
    } else {
        replace_or_append_value(env_entry, new_name, new_value);
    }
}

void add_or_update_to_env(char *name, char *value, t_env *env) {
    int i = 0;
    char *new_name = process_variable(name, env);
    char *new_value = value ? process_variable(value, env) : NULL;
    printf("new name : %s \nnew value: %s\n\n", new_name, new_value);

    if (!new_name || (value && !new_value)) {
        handle_memory_errors(new_name, new_value);
        return;  // Handle allocation error
    }

    // Assume check_semicolon is defined elsewhere
    check_semicolon(new_name, &new_value);
    int check_input_status = check_input(new_name);

    if (check_input_status == 0 || (value != NULL && new_value != NULL && !check_value(new_value))) {
        handle_memory_errors(new_name, new_value);
        return;
    }

    if (check_input_status == -1) {
        size_t len = strlen(new_name);
        if (new_name[len - 1] == '+') {
            new_name[len - 1] = '\0';  // Remove the '+' for proper name matching
        }
    }

    while (env->env[i] != NULL) {
        if (strncmp(env->env[i], new_name, strlen(new_name)) == 0 &&
            (env->env[i][strlen(new_name)] == '=' || env->env[i][strlen(new_name)] == '\0')) {
            update_env_variable(new_name, new_value, &env->env[i], check_input_status);
            handle_memory_errors(new_name, new_value);
            return;
        }
        i++;
    }

    // Variable not found, add it
    char **new_env = realloc(env->env, sizeof(char *) * (i + 2));
    if (!new_env) {
        handle_memory_errors(new_name, new_value);
        return;
    }
    env->env = new_env;

    replace_or_append_value(&env->env[i], new_name, new_value);
    env->env[i + 1] = NULL;
    handle_memory_errors(new_name, new_value);
}
*/

//akbrr size
/*
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
                else
                {
                    // If current_value is NULL or empty, create a new entry
                    printf("mafrud hon\n");

                    // Create a new environment variable with the new name and value
                    size_t new_len = name_len + (new_value ? strlen(new_value) : 0) + 1; // +1 for null terminator
                    env->env[i] = malloc(new_len + 2); // +2 for '=' and null terminator
                    if (!env->env[i])
                    {
                        free(new_name);
                        free(new_value);
                        return;  // Memory allocation error
                    }
                    if (new_value)
                        snprintf(env->env[i], new_len + 2, "%s=%s", new_name, new_value);
                    else
                        snprintf(env->env[i], new_len + 2, "%s=", new_name); // If new_value is NULL, just set it to the name

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
*/

