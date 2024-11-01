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
        return (NULL);
    
    // Check if there is a dollar sign and it's not escaped
    //hay awwl existence ll $
    if (strchr(input, '$') && !strstr(input, "\\$"))
    {
        size_t total_size = 1;  // Start with 1 for null terminator
        //el total size heye ll  overall size ll kelme
        char *start = input;
        //saving wen bblesh el start
        char *dollar = strchr(input, '$');
        //wen awl existence ll $
        char *new_str = malloc(total_size);
        //new str hwi el new string in which i will save everything fia
        if (!new_str)
            return (NULL);
        new_str[0] = '\0';  // Start with an empty string
        while (dollar)
        {
            // Append the part of the string before the dollar sign
            // if(strchr(start, '\'')!= NULL)
            //     strncat(new_str, start - 1, dollar - start -1); //export s$USER$99HOMESAL'$USER'$USER+=hhehee
            // else
                strncat(new_str, start, dollar - start );
            total_size += (dollar - start);

            //el value ll $ baed el $ yaani $USER  : USER
            char *var_name = dollar + 1;
            char *end_of_var = strpbrk(var_name, " '$?1234567890+\"");
            char temp_char = '\0';
            char first_char = *var_name; // Save the first character of the variable name
                    // printf("INITIAL start: %s\n",start);
                    // printf("INITIAL dollar: %s\n",dollar);
                    // printf("INITIAL var name: %s\n",var_name);
                    // printf("INITIAL end of var: %s\n\n\n",end_of_var);
            if (end_of_var)
            {
                temp_char = *end_of_var;
                *end_of_var = '\0';  // Temporarily terminate the variable name
               // printf("temp_char: %c\n",temp_char);
               // printf("end of var: %s\n\n\n",end_of_var);

            }
            //why is the prev part neccessary?

            // New condition: if the previous character is a single quote
            // Check if the first character is '?'
            if (first_char == '?')
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
            else if (dollar > input && *(dollar - 1) == '\'' && temp_char=='\'')
            {
                if (dollar)
                {
                    total_size += strlen(dollar);
                    new_str = realloc(new_str, total_size);
                    if (!new_str)
                        return (NULL); // Handle allocation failure
                   // printf("BEF: new str : %s\n",new_str);
                    strncat(new_str, dollar, strlen(dollar));
                   // printf("$$$$$$$$$$$$DOLLAR: %s\n",dollar);
                   // printf("AFT:new str : %s\n",new_str);
                    start = end_of_var + 1;
                   // printf("start: %s\n",start);
                  //  printf("end of var: %s\n\n\n",end_of_var);
                    if (*start == '\0')
                        break;  // Exit the loop if we've reached the end
                }
                else
                {
                    // If no closing quote is found, append the rest of the string and break
                    strcat(new_str, dollar);
                    break;
                }
              //  printf("current new str in els eif: %s\n",new_str);
                
            }
            else if (isdigit(first_char))
                start = var_name + 1;  // Skip the digit (which is just one character)
            else
            {
                char *env_value = get_env_value(env, var_name);
               // printf("in else the value is : %s \n env val is: %s\n\n",var_name,env_value);
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
            //printf("current new str: %s\n",new_str);
        }
        strcat(new_str, start);  // Append the remainder of the string
    //    printf("new-str: %s\n",new_str);
        return (remove_quotes_new_new(new_str));
    }
    // printf("input: %s\n",input);
    return (remove_quotes_new_new(strdup(input)));  // Return a copy of input if no processing is needed
}

/*
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
            char *end_of_var = strpbrk(var_name, " $?1234567890+'\""); // Find the end of variable name
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
                // Retrieve the value from the environment
                char *env_value = get_env_value(env, var_name);
                // If the variable is not found, treat it as an empty string
                if (env_value == NULL)
                    env_value = "";  // Treat it as empty string
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
                    start = var_name + strlen(var_name);  // Move past the variable name
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
        
        return (remove_quotes(new_str));
    }
    return (remove_quotes(strdup(input)));  // Return a copy of input if no processing is needed
}
*/

//tosgheer el process variable

// typedef struct {
//     char *new_str;
//     size_t total_size;
// } t_var_process;


// char *handle_env_variables(t_var_process *vp, char *var_name, char *end_of_var, t_env *env)
// {
//     char *env_value = get_env_value(env, var_name);
//     if (!env_value)
//         env_value = "";  // Treat it as empty string

//     vp->total_size += strlen(env_value);
//     vp->new_str = realloc(vp->new_str, vp->total_size);

//     if (vp->new_str)
//         strcat(vp->new_str, env_value);

//     if (end_of_var)
//     {
//         *end_of_var = '\0';  // Restore the character if changed
//         return end_of_var;  // Move start to the position after the variable
//     }

//     return var_name + strlen(var_name);  // Move past the variable name
// }

// char *handle_exit_status(t_var_process *vp, char *end_of_var)
// {
//     char exit_status[4];
//     snprintf(exit_status, sizeof(exit_status), "%d", global_var);

//     vp->total_size += strlen(exit_status);
//     vp->new_str = realloc(vp->new_str, vp->total_size);

//     if (vp->new_str)
//         strcat(vp->new_str, exit_status);

//     if (end_of_var)
//         return end_of_var + 1;  // Move past the variable

//     return end_of_var;  // No more characters
// }

// char *process_var_after_dollar(t_var_process *vp, char *dollar, t_env *env)
// {
//     char *var_name = dollar + 1;
//     char *end_of_var = strpbrk(var_name, " $?1234567890");
//     char first_char = *var_name;

//     if (first_char == '?')
//         return handle_exit_status(vp, end_of_var);
//     else if (isdigit(first_char))
//         return var_name + 1;  // Skip the digit
//     else
//         return handle_env_variables(vp, var_name, end_of_var, env);
// }

// void append_before_dollar(t_var_process *vp, char *start, char *dollar)
// {
//     size_t length_before_dollar = dollar - start;
//     vp->total_size += length_before_dollar;
//     vp->new_str = realloc(vp->new_str, vp->total_size);

//     if (vp->new_str)
//         strncat(vp->new_str, start, length_before_dollar);
// }

// char *handle_dollar_in_input(char *input, t_env *env)
// {
//     t_var_process vp;
//     vp.total_size = 1;
//     vp.new_str = malloc(vp.total_size);

//     if (!vp.new_str)
//         return (NULL);

//     vp.new_str[0] = '\0';
//     char *start = input;
//     char *dollar = strchr(input, '$');

//     while (dollar)
//     {
//         append_before_dollar(&vp, start, dollar);
//         start = process_var_after_dollar(&vp, dollar, env);  // Get updated start
//         dollar = strchr(start, '$');

//         if (dollar != NULL && *(dollar + 1) == '\0')
//             break;
//     }

//     strcat(vp.new_str, start);  // Append remainder of the string
//     return vp.new_str;
// }


// char *process_variable(char *input, t_env *env)
// {
//     if (input == NULL)
//         return (NULL);

//     if (strchr(input, '$') && !strstr(input, "\\$"))
//         return handle_dollar_in_input(input, env);

//     return strdup(input);  // Return a copy of input if no processing is needed
// }
