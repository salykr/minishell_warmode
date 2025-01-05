/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdennaou <rdennaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:18:23 by marvin            #+#    #+#             */
/*   Updated: 2024/12/26 13:22:51 by rdennaou         ###   ########.fr       */
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
char *process_variable(char *input, t_env *env)
{
    t_context ctx;
    ctx.input = input;
    if (input == NULL)
        return (NULL);
    if (strchr(ctx.input, '$'))
    {
        ctx.total_size = pv_initialise_vars(&ctx);
        while (ctx.dollar)
        {
            if (pv_backslashes_cases(&ctx))
            {
                ctx.start = ctx.dollar + 1;
                ctx.dollar = strchr(ctx.start, '$');
                continue;  // Skip the rest of the loop iteration
            }
            if(ctx.dollar != ctx.input)
            {
                if (ctx.start[ctx.dollar - ctx.start - 1] == '\'') 
                    pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + ctx.dollar - ctx.start, ctx.start, ctx.dollar - ctx.start - 1);
                else
                    pv_resize_concat(&(ctx.new_str),strlen(ctx.new_str) + ctx.dollar - ctx.start + 1, ctx.start, ctx.dollar - ctx.start);
            }
            pv_fill_values(&ctx);
            if (ctx.end_of_var)
            {
                ctx.temp_char = *(ctx.end_of_var);
                *(ctx.end_of_var) = '\0';
            }
            if (ctx.first_char == '?')
            {
                if (!pv_question_mark(&ctx))
                    break;  // Exit the loop if there was an issue or no further processing is needed
            }
            else if (ctx.dollar > ctx.input && *(ctx.dollar - 1) == '\'' && ctx.temp_char == '\'')
            {
                if (ctx.dollar)
                {
                    ctx.total_size += strlen(ctx.dollar);
                    pv_resize_concat(&(ctx.new_str), ctx.total_size, ctx.dollar,  strlen(ctx.dollar));
                    ctx.start = ctx.end_of_var + 1;
                    if (*(ctx.start) == '\0')
                        break;
                }
                else
                {
                    pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.dollar) + 1, ctx.dollar, (size_t)-1);
                    break;
                }
            }
            else if (isdigit(ctx.first_char))
                ctx.start = ctx.var_name + 1;
            else
            {
                ctx.env_value = get_env_value(env, ctx.var_name);
                if (ctx.env_value == NULL)
                    ctx.env_value = "";
                ctx.total_size += strlen(ctx.env_value);
                pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.env_value) + 1, ctx.env_value, (size_t)-1);
                if (ctx.end_of_var)
                {
                    *(ctx.end_of_var) = ctx.temp_char;
                    ctx.start = ctx.end_of_var;
                }
                else
                    ctx.start = ctx.var_name + strlen(ctx.var_name);
                if(ctx.env_value!=NULL && *(ctx.env_value)!='\0')
                    free(ctx.env_value);
            }
            ctx.dollar = strchr(ctx.start, '$');
            if (ctx.dollar != NULL && *(ctx.dollar + 1) == '\0')
                break;
        }
        pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.start) + 1, ctx.start, (size_t)-1);
        
        return (remove_paired_quotes(ctx.new_str));
    }
    return (remove_paired_quotes(strdup(input)));
}


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
char *process_variable2(char *input, t_env *env) {
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
