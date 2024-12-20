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

// // Handle single quotes
// static void pv_handle_single_quotes(t_context *ctx) {
//     if (ctx->start[ctx->dollar - ctx->start - 1] == '\'') {
//         pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + ctx->dollar - ctx->start, ctx->start, ctx->dollar - ctx->start - 1);
//     } else {
//         pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + ctx->dollar - ctx->start + 1, ctx->start, ctx->dollar - ctx->start);
//     }
// }

// // Process the variable and replace
// static void pv_process_variable(t_context *ctx) {
//     pv_fill_values(ctx);
//     if (ctx->end_of_var) {
//         ctx->temp_char = *(ctx->end_of_var);
//         *(ctx->end_of_var) = '\0';
//     }
// }

// // Handle variable replacement logic
// static void pv_replace_variable(t_context *ctx)
// {
//     if (ctx->first_char == '?')
//     {
//         if (!pv_question_mark(ctx))
//             return;
//     }
//     else if (isdigit(ctx->first_char))
//         ctx->start = ctx->var_name + 1;
//     else
//     {
//         ctx->env_value = get_env_value(&(ctx->env), ctx->var_name);
//         if (ctx->env_value == NULL)
//             ctx->env_value = "";
//         ctx->total_size += strlen(ctx->env_value);
//         pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + strlen(ctx->env_value) + 1, ctx->env_value, (size_t)-1);
//         if (ctx->end_of_var)
//         {
//             *(ctx->end_of_var) = ctx->temp_char;
//             ctx->start = ctx->end_of_var;
//         }
//         else
//             ctx->start = ctx->var_name + strlen(ctx->var_name);
//         if (ctx->env_value && *(ctx->env_value) != '\0')
//             free(ctx->env_value);
//     }
// }

// // Process dollar sign cases
// static void pv_process_dollar_sign(t_context *ctx)
// {
//     while (ctx->dollar)
//     {
//         if (pv_handle_backslashes(ctx)) continue;
//         if (ctx->dollar != ctx->input)
//             pv_handle_single_quotes(ctx);
//         pv_process_variable(ctx);
//         pv_replace_variable(ctx);
//         ctx->dollar = strchr(ctx->start, '$');
//         if (ctx->dollar && *(ctx->dollar + 1) == '\0') break;
//     }
// }

// // Main function to process variable
// char *process_variable(char *input, t_env *env)
// {
//     if (input == NULL)
//         return NULL;
//     t_context ctx;
//     pv_initialize_context(&ctx, input, env);
//     if (ctx.dollar)
//     {
//         ctx.total_size = pv_initialise_vars(&ctx);
//         pv_process_dollar_sign(&ctx);
//         pv_resize_concat(&(ctx.new_str), strlen(ctx.new_str) + strlen(ctx.start) + 1, ctx.start, (size_t)-1);
//         return remove_paired_quotes(ctx.new_str);
//     }
//     return remove_paired_quotes(strdup(input));
// }



// char *process_variable2(char *input, t_env *env)
// {
//     if (input == NULL)
//         return NULL;
//     char *dollar;
//     char *start;
//     char *env_value;
//     char *var_name;
//     char *new_str;
//     dollar = strchr(input, '$');
//     if(dollar == NULL)
//         return(strdup(input));
//     start = input;
//     new_str = malloc (1);
//     if (!new_str) // Check if malloc failed
//         return NULL; // Indicate error
//     (new_str)[0] = '\0'; // Initialize as an empty string
//     if(start - 1 >= input)
//         pv_resize_concat(&new_str, dollar - start  + 1, dollar - start,(size_t) - 1);
//     while(dollar)
//     {
//         if(strchr(input, '$') != dollar)//yaani msh awwl call
//             dollar = strchr(start, '$');
//         if(isdigit(dollar + 1))
//         {
//             start = dollar + 2;
//             continue;
//         }
//         var_name = strpbrk(dollar, " ./'$%[],:-+=/{}@!?#\"\\+");
//         if(start - 1 >= input && *(start - 1) != '\'')
//         {
//             env_value = get_env_value(env, var_name);
//             if (env_value == NULL)
//                 env_value = "";        
//             pv_resize_concat(&new_str, strlen(new_str) + strlen(env_value) + 1, env_value, (size_t)-1);
//             if (env_value && *(env_value) != '\0')
//                 free(env_value);
//             start = 
//         }
//         return(new_str);

//     }
// }