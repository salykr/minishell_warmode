/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:03:06 by marvin            #+#    #+#             */
/*   Updated: 2024/11/18 17:03:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"

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

void pv_fill_values(t_context *ctx)
{
    ctx->total_size += (ctx->dollar - ctx->start);
    ctx->var_name = ctx->dollar + 1;
    ctx->end_of_var = strpbrk(ctx->var_name, " ./'\"$?1234567890+\\");
    if(ctx->end_of_var != NULL && (strcmp(ctx->end_of_var ,"\"")|| strcmp(ctx->end_of_var ,"\"")))
        ctx->end_of_var = ctx->end_of_var + 1;
    // printf("here: %s\n", ctx->end_of_var);
    if (ctx->end_of_var != NULL && ft_isdigit((unsigned char)*(ctx->end_of_var)))
    {
        if(strcmp(ctx->end_of_var - 1,"$"))
            ctx->end_of_var = strpbrk(ctx->var_name, " /'$?+\"");
    }
    ctx->temp_char = '\0';
    ctx->first_char = *(ctx->var_name);
}

// Function to initialize the context
void pv_initialize_context(t_context *ctx, char *input, t_env *env) {
    ctx->input = input;
    ctx->env = *env;
    ctx->new_str = NULL;
    ctx->total_size = 0;
    ctx->dollar = strchr(input, '$');
}

// Handle backslash cases
int pv_handle_backslashes(t_context *ctx)
{
    if (pv_backslashes_cases(ctx)) {
        ctx->start = ctx->dollar + 1;
        ctx->dollar = strchr(ctx->start, '$');
        return 1;
    }
    return 0;
}