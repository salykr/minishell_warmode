/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5_1.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:01:07 by marvin            #+#    #+#             */
/*   Updated: 2024/11/18 17:01:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"


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
