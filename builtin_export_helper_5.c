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

char *resize_string(char *str, size_t new_size)
{
    if (!str || new_size == 0) {
        return NULL;  // Ensure the input is valid
    }
    // Allocate new memory with the specified new size
    char *new_str = (char *)malloc(new_size);
    if (!new_str) {
        free(str);  // Free the old string to prevent leaks
        return NULL;  // Return NULL if memory allocation fails
    }

    // Copy the old content into the new allocated memory
    strncpy(new_str, str, new_size - 1);
    new_str[new_size - 1] = '\0';  // Ensure null termination

    // Free the old memory
    free(str);
    return new_str;  // Return the resized string
}

typedef struct s_context{
    size_t total_size;  // Start with 1 for null terminator
    char *start;
    char *dollar;
    char *new_str;
    char *var_name;
    char *end_of_var;
    char temp_char;
    char first_char;
    char *input;
    t_env env;
    char *env_value;
}t_context;

size_t pv_count_backslashes(t_context *ctx)
{
    size_t backslash_count;
    char *p;

    p = ctx->dollar - 1;
    backslash_count = 0;
    if (ctx->dollar != ctx->input)  // Ensure dollar is not the first character
    {
        while (p >= ctx->input && *p == '\\')
       {
            backslash_count++;
            p--;    
       }
    }
    // 
    // //check for ;loop
    //     if (dollar != input)  // Ensure dollar is not the first character
    //         {
    //             // printf("\n\n\nentered\n\n\n\n\n\n");
    //             for (char *p = dollar - 1; p >= input && *p == '\\'; p--)
    //                 backslash_count++;
    //         }
    // 
    return (backslash_count);
}
 //resized str //len of resize                      //concqt //soze of concat 
void pv_resize_concat(char **resized_str, size_t len_resize, char *concat_str, size_t len_concat)
{
    // printf("Before resizing: resized_str='%s', len_resize=%zu, len_concat=%zu\n", *resized_str, len_resize, len_concat);
    *resized_str = resize_string(*resized_str, len_resize);
    // printf("After resizing: resized_str='%s'\n", *resized_str);

    if (len_concat == (size_t)-1)
        strcat(*resized_str, concat_str);
    else
        strncat(*resized_str, concat_str, len_concat);

    // printf("After concatenation: resized_str='%s'\n", *resized_str);

}
int pv_initialise_vars(t_context *ctx)
{
    if (!ctx->input) // Check if input is NULL
        return -1; // Indicate error

    ctx->start = ctx->input;
    ctx->dollar = strchr(ctx->input, '$');
    ctx->new_str = malloc(1);
    if (!ctx->new_str) // Check if malloc failed
        return -1; // Indicate error
    (ctx->new_str)[0] = '\0'; // Initialize as an empty string
    return 1; // Indicate success
}
int pv_backslashes_cases(t_context *ctx)
{
    size_t backslash_count = pv_count_backslashes(ctx);

    if (backslash_count % 2 != 0)
    {
        pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + ctx->dollar - ctx->start + 2, ctx->start, ctx->dollar - ctx->start + 1);
        ctx->total_size += (ctx->dollar - ctx->start + 1);
        ctx->new_str = realloc(ctx->new_str, ctx->total_size);
        if (!ctx->new_str)
            return (0);  // Return 0 to indicate failure
        return 1;  // Return 1 to indicate success
    }
    return 0;  // No backslash handling needed
}
int pv_question_mark(t_context *ctx)
{
    char *exit_status = ft_itoa(global_var);
    if (!exit_status)
        return (0);  // Return 0 if memory allocation fails
    
    ctx->total_size += strlen(exit_status);
    pv_resize_concat(&(ctx->new_str), ctx->total_size, exit_status, (size_t)-1);
    free(exit_status);

    if (ctx->end_of_var)
        ctx->start = ctx->end_of_var + 1;
    else
        return (0);  // Return 0 to indicate the loop should break (no further processing)
    return 1;  // Return 1 to indicate success
}
int pv_update_pointers(t_context *ctx)
{
    if (ctx->end_of_var)
    {
        *(ctx->end_of_var) = ctx->temp_char;
        ctx->start = ctx->end_of_var;  // Update start if end_of_var is present
    }
    else
        ctx->start = ctx->var_name + strlen(ctx->var_name);  // Otherwise, move start past the variable name
    return 1;  // Return 1 to indicate success
}

int pv_env_variable(t_context *ctx, char *env_value)
{
    if (env_value == NULL)
        env_value = "";  // Use empty string if no value is found
    ctx->total_size += strlen(env_value);  // Update total size based on the length of env_value
    pv_resize_concat(&(ctx->new_str), ctx->total_size, env_value, (size_t)-1);  // Concatenate env_value to new_str
    return 1;  // Return 1 to indicate success
}
// typedef struct s_context{
//     size_t total_size;  // Start with 1 for null terminator
//     char *start;
//     char *dollar;
//     char *new_str;
//     char *var_name;
//     char *end_of_var;
//     char temp_char;
//     char first_char;
//     char *input;
//     t_env env;
// }t_context;
void pv_fill_values(t_context *ctx)
{
    ctx->total_size += (ctx->dollar - ctx->start);
    ctx->var_name = ctx->dollar + 1;
    ctx->end_of_var = strpbrk(ctx->var_name, " '$?1234567890+\"");
    ctx->temp_char = '\0';
    ctx->first_char = *(ctx->var_name);
}
// char *process_variable(char *input, t_env *env)
// {
//     t_context ctx;
//     ctx.input = input;
//     if (input == NULL)
//         return (NULL);
//     if (strchr(ctx.input, '$'))
//     {
//         ctx.total_size = pv_initialise_vars(&ctx);
//         while (ctx.dollar)
//         {
//             if (pv_backslashes_cases(&ctx))
//             {
//                 ctx.start = ctx.dollar + 1;
//                 ctx.dollar = strchr(ctx.start, '$');
//                 continue;  // Skip the rest of the loop iteration
//             }
//             if(ctx.dollar != ctx.input)
//             {
//                 if (ctx.start[ctx.dollar - ctx.start - 1] == '\'') 
//                     pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + ctx.dollar - ctx.start, ctx.start, ctx.dollar - ctx.start - 1);
//                 else
//                     pv_resize_concat(&(ctx.new_str),strlen(ctx.new_str) + ctx.dollar - ctx.start + 1, ctx.start, ctx.dollar - ctx.start);
//             }
//             pv_fill_values(&ctx);
//             if (ctx.end_of_var)
//             {
//                 ctx.temp_char = *(ctx.end_of_var);
//                 *(ctx.end_of_var) = '\0';
//             }
//             if (ctx.first_char == '?')
//             {
//                 if (!pv_question_mark(&ctx))
//                     break;  // Exit the loop if there was an issue or no further processing is needed
//             }
//             else if (ctx.dollar > ctx.input && *(ctx.dollar - 1) == '\'' && ctx.temp_char == '\'')
//             {
//                 if (ctx.dollar)
//                 {
//                     ctx.total_size += strlen(ctx.dollar);
//                     pv_resize_concat(&(ctx.new_str), ctx.total_size, ctx.dollar,  strlen(ctx.dollar));
//                     ctx.start = ctx.end_of_var + 1;
//                     if (*(ctx.start) == '\0')
//                         break;
//                 }
//                 else
//                 {
//                     pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.dollar) + 1, ctx.dollar, (size_t)-1);
//                     break;
//                 }
//             }
//             else if (isdigit(ctx.first_char))
//                 ctx.start = ctx.var_name + 1;
//             else
//             {
//                 ctx.env_value = get_env_value(env, ctx.var_name);
//                 if (ctx.env_value == NULL)
//                     ctx.env_value = "";
//                 ctx.total_size += strlen(ctx.env_value);
//                 pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.env_value) + 1, ctx.env_value, (size_t)-1);
//                 if (ctx.end_of_var)
//                 {
//                     *(ctx.end_of_var) = ctx.temp_char;
//                     ctx.start = ctx.end_of_var;
//                 }
//                 else
//                     ctx.start = ctx.var_name + strlen(ctx.var_name);
//                 if(ctx.env_value!=NULL && *(ctx.env_value)!='\0')
//                     free(ctx.env_value);
//             }
//             ctx.dollar = strchr(ctx.start, '$');
//             if (ctx.dollar != NULL && *(ctx.dollar + 1) == '\0')
//                 break;
//         }
//         pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.start) + 1, ctx.start, (size_t)-1);
//         return (remove_paired_quotes(ctx.new_str));
//     }
//     return (remove_paired_quotes(strdup(input)));
// }


// Function to initialize the context
static void pv_initialize_context(t_context *ctx, char *input, t_env *env) {
    ctx->input = input;
    ctx->env = *env;
    ctx->new_str = NULL;
    ctx->total_size = 0;
    ctx->dollar = strchr(input, '$');
}

// Handle backslash cases
static int pv_handle_backslashes(t_context *ctx) {
    if (pv_backslashes_cases(ctx)) {
        ctx->start = ctx->dollar + 1;
        ctx->dollar = strchr(ctx->start, '$');
        return 1;
    }
    return 0;
}

// Handle single quotes
static void pv_handle_single_quotes(t_context *ctx) {
    if (ctx->start[ctx->dollar - ctx->start - 1] == '\'') {
        pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + ctx->dollar - ctx->start, ctx->start, ctx->dollar - ctx->start - 1);
    } else {
        pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + ctx->dollar - ctx->start + 1, ctx->start, ctx->dollar - ctx->start);
    }
}

// Process the variable and replace
static void pv_process_variable(t_context *ctx) {
    pv_fill_values(ctx);
    if (ctx->end_of_var) {
        ctx->temp_char = *(ctx->end_of_var);
        *(ctx->end_of_var) = '\0';
    }
}

// Handle variable replacement logic
static void pv_replace_variable(t_context *ctx) {
    if (ctx->first_char == '?') {
        if (!pv_question_mark(ctx)) return;
    } else if (isdigit(ctx->first_char)) {
        ctx->start = ctx->var_name + 1;
    } else {
        ctx->env_value = get_env_value(&(ctx->env), ctx->var_name);
        if (ctx->env_value == NULL) ctx->env_value = "";
        ctx->total_size += strlen(ctx->env_value);
        pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + strlen(ctx->env_value) + 1, ctx->env_value, (size_t)-1);
        if (ctx->end_of_var) {
            *(ctx->end_of_var) = ctx->temp_char;
            ctx->start = ctx->end_of_var;
        } else {
            ctx->start = ctx->var_name + strlen(ctx->var_name);
        }
        if (ctx->env_value && *(ctx->env_value) != '\0') free(ctx->env_value);
    }
}

// Process dollar sign cases
static void pv_process_dollar_sign(t_context *ctx) {
    while (ctx->dollar) {
        if (pv_handle_backslashes(ctx)) continue;
        if (ctx->dollar != ctx->input) pv_handle_single_quotes(ctx);
        pv_process_variable(ctx);
        pv_replace_variable(ctx);
        ctx->dollar = strchr(ctx->start, '$');
        if (ctx->dollar && *(ctx->dollar + 1) == '\0') break;
    }
}

// Main function to process variable
char *process_variable(char *input, t_env *env) {
    if (input == NULL) return NULL;

    t_context ctx;
    pv_initialize_context(&ctx, input, env);

    if (ctx.dollar) {
        ctx.total_size = pv_initialise_vars(&ctx);
        pv_process_dollar_sign(&ctx);
        pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.start) + 1, ctx.start, (size_t)-1);
        return remove_paired_quotes(ctx.new_str);
    }
    return remove_paired_quotes(strdup(input));
}

//before tosgheer
/*char *process_variable(char *input, t_env *env)
{
    if (input == NULL)
        return (NULL);

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
            size_t backslash_count = 0;
            
           if (dollar != input)  // Ensure dollar is not the first character
            {
                // printf("\n\n\nentered\n\n\n\n\n\n");
                for (char *p = dollar - 1; p >= input && *p == '\\'; p--)
                    backslash_count++;
            }
            // If odd number of backslashes, append dollar and skip expansion
            if (backslash_count % 2 != 0)
            {
                new_str=resize_string(new_str,strlen(new_str) + dollar - start + 2);
                strncat(new_str, start, dollar - start + 1);
                total_size += (dollar - start + 1);
                new_str = realloc(new_str, total_size);
                if (!new_str)
                    return (NULL);
                start = dollar + 1;
                dollar = strchr(start, '$');
                continue;
            }
            if(dollar != input)
            {

                if (start[dollar - start - 1] == '\'') 
                {
                    new_str=resize_string(new_str,strlen(new_str) + dollar - start );                
                    strncat(new_str, start, dollar - start - 1);
                }
                else
                {
                    new_str=resize_string(new_str,strlen(new_str) + dollar - start + 1);
                    // printf("!!!!!!!!!!!!!!!!!!!!\n\n!!!!!!!!!!!!!!!!!!!!!!!\n\n");
                    strncat(new_str, start, dollar - start);
                }
            }
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
                     new_str=resize_string(new_str,strlen(new_str) + strlen(dollar) + 1);
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
                printf("len is %ld\n",strlen(env_value));
                new_str = realloc(new_str, total_size);
                if (!new_str)
                    return (NULL);
                 new_str=resize_string(new_str,strlen(new_str) + strlen(env_value) + 1);
                strcat(new_str, env_value);
                if (end_of_var)
                {
                    *end_of_var = temp_char;
                    start = end_of_var;
                }
                else
                    start = var_name + strlen(var_name);
                printf("the env is: %s\n",env_value);
                if(env_value!=NULL && *env_value!='\0')
                    free(env_value);
            }

            dollar = strchr(start, '$');
            if (dollar != NULL && *(dollar + 1) == '\0')
                break;
        }
         new_str=resize_string(new_str,strlen(new_str) + strlen(start) + 1);
        strcat(new_str, start);
        printf("the new_str: %s!\n\n",new_str);
        return (remove_paired_quotes(new_str));
    }
    printf("the new_str: %s!\n\n",input);
    return (remove_paired_quotes(strdup(input)));
}
*/


//before leaks
// char *process_variable(char *input, t_env *env)
// {
//     if (input == NULL)
//         return (NULL);
    
//     // Check if there is a dollar sign and it's not escaped
//     //hay awwl existence ll $
//     if (strchr(input, '$') && !strstr(input, "\\$"))
//     {
//         size_t total_size = 1;  // Start with 1 for null terminator
//         //el total size heye ll  overall size ll kelme
//         char *start = input;
//         //saving wen bblesh el start
//         char *dollar = strchr(input, '$');
//         //wen awl existence ll $
//         char *new_str = malloc(total_size);
//         //new str hwi el new string in which i will save everything fia
//         if (!new_str)
//             return (NULL);
//         new_str[0] = '\0';  // Start with an empty string
//         while (dollar)
//         {
//             // Append the part of the string before the dollar sign
//             // if(strchr(start, '\'')!= NULL)
//             //     strncat(new_str, start - 1, dollar - start -1); //export s$USER$99HOMESAL'$USER'$USER+=hhehee
//             // else
//                 strncat(new_str, start, dollar - start );
//             total_size += (dollar - start);

//             //el value ll $ baed el $ yaani $USER  : USER
//             char *var_name = dollar + 1;
//             char *end_of_var = strpbrk(var_name, " '$?1234567890+\"");
//             char temp_char = '\0';
//             char first_char = *var_name; // Save the first character of the variable name
//                     // printf("INITIAL start: %s\n",start);
//                     // printf("INITIAL dollar: %s\n",dollar);
//                     // printf("INITIAL var name: %s\n",var_name);
//                     // printf("INITIAL end of var: %s\n\n\n",end_of_var);
//             if (end_of_var)
//             {
//                 temp_char = *end_of_var;
//                 *end_of_var = '\0';  // Temporarily terminate the variable name
//                // printf("temp_char: %c\n",temp_char);
//                // printf("end of var: %s\n\n\n",end_of_var);

//             }
//             //why is the prev part neccessary?

//             // New condition: if the previous character is a single quote
//             // Check if the first character is '?'
//             if (first_char == '?')
//             {
//                 char exit_status[4];
//                 snprintf(exit_status, sizeof(exit_status), "%d", global_var);

//                 total_size += strlen(exit_status);
//                 new_str = realloc(new_str, total_size);  // Resize the string to fit the new value
//                 if (!new_str)
//                     return (NULL);
//                 strcat(new_str, exit_status);  // Concatenate the exit status
//                      // Move start to the position after the variable
//                 if (end_of_var)
//                     start = end_of_var + 1;  // Move past the variable
//                 else
//                     break;  // No more characters after `$?`
//             }
//             else if (dollar > input && *(dollar - 1) == '\'' && temp_char=='\'')
//             {
//                 if (dollar)
//                 {
//                     total_size += strlen(dollar);
//                     new_str = realloc(new_str, total_size);
//                     if (!new_str)
//                         return (NULL); // Handle allocation failure
//                    // printf("BEF: new str : %s\n",new_str);
//                     strncat(new_str, dollar, strlen(dollar));
//                    // printf("$$$$$$$$$$$$DOLLAR: %s\n",dollar);
//                    // printf("AFT:new str : %s\n",new_str);
//                     start = end_of_var + 1;
//                    // printf("start: %s\n",start);
//                   //  printf("end of var: %s\n\n\n",end_of_var);
//                     if (*start == '\0')
//                         break;  // Exit the loop if we've reached the end
//                 }
//                 else
//                 {
//                     // If no closing quote is found, append the rest of the string and break
//                     strcat(new_str, dollar);
//                     break;
//                 }
//               //  printf("current new str in els eif: %s\n",new_str);
                
//             }
//             else if (isdigit(first_char))
//                 start = var_name + 1;  // Skip the digit (which is just one character)
//             else
//             {
//                 char *env_value = get_env_value(env, var_name);
//                // printf("in else the value is : %s \n env val is: %s\n\n",var_name,env_value);
//                 if (env_value == NULL)
//                     env_value = "";  // Treat it as an empty string
//                 total_size += strlen(env_value);
//                 new_str = realloc(new_str, total_size);  // Resize the string to fit the new value
//                 if (!new_str)
//                     return (NULL);
//                 strcat(new_str, env_value);
//                 if (end_of_var)
//                 {
//                     *end_of_var = temp_char;
//                     start = end_of_var;
//                 }
//                 else
//                     start = var_name + strlen(var_name);
//             }
//             dollar = strchr(start, '$');
//             if (dollar != NULL && *(dollar + 1) == '\0')
//                 break;
//             //printf("current new str: %s\n",new_str);
//         }
//         strcat(new_str, start);  // Append the remainder of the string
//     //    printf("new-str: %s\n",new_str);
//         return (remove_quotes_new_new(new_str));
//     }
//     // printf("input: %s\n",input);
//     return (remove_quotes_new_new(strdup(input)));  // Return a copy of input if no processing is needed
// }

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
/*

size_t pv_count_backslashes(char *dollar, char *input)
{
    size_t backslash_count;
    char *p;

    p = dollar - 1;
    backslash_count = 0;
    if (dollar != input)  // Ensure dollar is not the first character
    {
        while (p >= input && *p == '\\')
       {
            backslash_count++;
            p--;    
       }
    }
    // 
    // //check for ;loop
    //     if (dollar != input)  // Ensure dollar is not the first character
    //         {
    //             // printf("\n\n\nentered\n\n\n\n\n\n");
    //             for (char *p = dollar - 1; p >= input && *p == '\\'; p--)
    //                 backslash_count++;
    //         }
    // 
    return (backslash_count);
}
 //resized str //len of resize                      //concqt //soze of concat 
void pv_resize_concat(char **resized_str, size_t len_resize, char *concat_str, size_t len_concat)
{
    // printf("Before resizing: resized_str='%s', len_resize=%zu, len_concat=%zu\n", *resized_str, len_resize, len_concat);
    *resized_str = resize_string(*resized_str, len_resize);
    // printf("After resizing: resized_str='%s'\n", *resized_str);

    if (len_concat == (size_t)-1)
        strcat(*resized_str, concat_str);
    else
        strncat(*resized_str, concat_str, len_concat);

    // printf("After concatenation: resized_str='%s'\n", *resized_str);

}
int pv_initialise_vars(char **start, char **dollar, char **new_str, char *input)
{
    if (!input) // Check if input is NULL
        return -1; // Indicate error

    *start = input;
    *dollar = strchr(input, '$');
    *new_str = malloc(1);
    if (!*new_str) // Check if malloc failed
        return -1; // Indicate error

    (*new_str)[0] = '\0'; // Initialize as an empty string
    return 1; // Indicate success
}
int pv_backslashes_cases(char **new_str, size_t *total_size, char *start, char *dollar)
{
    size_t backslash_count = pv_count_backslashes(dollar, start);

    if (backslash_count % 2 != 0)
    {
        pv_resize_concat(new_str, strlen(*new_str) + dollar - start + 2, start, dollar - start + 1);
        *total_size += (dollar - start + 1);
        *new_str = realloc(*new_str, *total_size);
        if (!*new_str)
            return (0);  // Return 0 to indicate failure

        return 1;  // Return 1 to indicate success
    }
    return 0;  // No backslash handling needed
}
int pv_question_mark(char **new_str, size_t *total_size, char **start, char *end_of_var)
{
    char *exit_status = ft_itoa(global_var);
    if (!exit_status)
        return (0);  // Return 0 if memory allocation fails
    
    *total_size += strlen(exit_status);
    pv_resize_concat(new_str, *total_size, exit_status, (size_t)-1);
    free(exit_status);

    if (end_of_var)
        *start = end_of_var + 1;
    else
        return (0);  // Return 0 to indicate the loop should break (no further processing)
    return 1;  // Return 1 to indicate success
}
int pv_update_pointers(char **start, char *end_of_var, char temp_char, char *var_name)
{
    if (end_of_var)
    {
        *end_of_var = temp_char;
        *start = end_of_var;  // Update start if end_of_var is present
    }
    else
        *start = var_name + strlen(var_name);  // Otherwise, move start past the variable name
    return 1;  // Return 1 to indicate success
}

int pv_env_variable(char **new_str, size_t *total_size, char *env_value)
{
    if (env_value == NULL)
        env_value = "";  // Use empty string if no value is found
    *total_size += strlen(env_value);  // Update total size based on the length of env_value
    pv_resize_concat(new_str, *total_size, env_value, (size_t)-1);  // Concatenate env_value to new_str
    return 1;  // Return 1 to indicate success
}
typedef struct s_context{
    size_t total_size;  // Start with 1 for null terminator
    char *start;
    char *dollar;
    char *new_str;
    char *var_name;
    char *end_of_var;
    char temp_char;
    char first_char;
    char *input;
    t_env env;
}t_context;

char *process_variable(char *input, t_env *env)
{
    size_t total_size;  // Start with 1 for null terminator
    char *start;
    char *dollar;
    char *new_str;
    char *var_name;
    char *end_of_var;
    char temp_char;
    char first_char;
    t_context ctx;
    ctx.input = input;
    if (input == NULL)
        return (NULL);
    if (strchr(ctx.input, '$'))
    {
        ctx.total_size = pv_initialise_vars(&start, &dollar, &new_str, input);
        while (dollar)
        {
            if (pv_backslashes_cases(&new_str, &total_size, start, dollar))
            {
                start = dollar + 1;
                dollar = strchr(start, '$');
                continue;  // Skip the rest of the loop iteration
            }
            if(dollar != input)
            {
                if (start[dollar - start - 1] == '\'') 
                    pv_resize_concat(&new_str, strlen(new_str) + dollar - start, start, dollar - start - 1);
                else
                    pv_resize_concat(&new_str,strlen(new_str) + dollar - start + 1, start, dollar - start);
            }
            total_size += (dollar - start);
            var_name = dollar + 1;
            end_of_var = strpbrk(var_name, " '$?1234567890+\"");
            temp_char = '\0';
            first_char = *var_name;
            if (end_of_var)
            {
                temp_char = *end_of_var;
                *end_of_var = '\0';
            }
            if (first_char == '?')
            {
                if (!pv_question_mark(&new_str, &total_size, &start, end_of_var))
                    break;  // Exit the loop if there was an issue or no further processing is needed
            }
            else if (dollar > input && *(dollar - 1) == '\'' && temp_char == '\'')
            {
                if (dollar)
                {
                    total_size += strlen(dollar);
                    pv_resize_concat(&new_str, total_size, dollar,  strlen(dollar));
                    start = end_of_var + 1;
                    if (*start == '\0')
                        break;
                }
                else
                {
                    pv_resize_concat(&new_str, strlen(new_str) + strlen(dollar) + 1, dollar, (size_t)-1);
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
                pv_resize_concat(&new_str, strlen(new_str) + strlen(env_value) + 1, env_value, (size_t)-1);
                if (end_of_var)
                {
                    *end_of_var = temp_char;
                    start = end_of_var;
                }
                else
                    start = var_name + strlen(var_name);
                if(env_value!=NULL && *env_value!='\0')
                    free(env_value);
            }
            dollar = strchr(start, '$');
            if (dollar != NULL && *(dollar + 1) == '\0')
                break;
        }
        pv_resize_concat(&new_str, strlen(new_str) + strlen(start) + 1, start, (size_t)-1);
        return (remove_paired_quotes(new_str));
    }
    return (remove_paired_quotes(strdup(input)));
}
*/