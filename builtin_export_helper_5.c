/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdennaou <rdennaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:18:23 by marvin            #+#    #+#             */
/*   Updated: 2024/12/23 16:52:44 by rdennaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*resize_string(char *str, size_t new_size)
{
	if (!str || new_size == 0) {
		return (NULL);  // Ensure the input is valid
	}
	// Allocate new memory with the specified new size
	char *new_str = (char *)malloc(new_size);
	if (!new_str) {
		free(str);  // Free the old string to prevent leaks
		return (NULL);  // Return NULL if memory allocation fails
	}

	// Copy the old content into the new allocated memory
	strncpy(new_str, str, new_size - 1);
	new_str[new_size - 1] = '\0';  // Ensure null termination

	// Free the old memory
	free(str);
	return (new_str);  // Return the resized string
}

size_t	pv_count_backslashes(t_context *ctx)
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
void	pv_resize_concat(char **resized_str, size_t len_resize, char *concat_str, size_t len_concat)
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
int	pv_initialise_vars(t_context *ctx)
{
	if (!ctx->input) // Check if input is NULL
		return (-1); // Indicate error

	ctx->start = ctx->input;
	ctx->dollar = strchr(ctx->input, '$');
	ctx->new_str = malloc(1);
	if (!ctx->new_str) // Check if malloc failed
		return (-1); // Indicate error
	(ctx->new_str)[0] = '\0'; // Initialize as an empty string
	return (1); // Indicate success
}
int	pv_backslashes_cases(t_context *ctx)
{
	size_t backslash_count = pv_count_backslashes(ctx);

	if (backslash_count % 2 != 0)
	{
		pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + ctx->dollar - ctx->start + 2, ctx->start, ctx->dollar - ctx->start + 1);
		ctx->total_size += (ctx->dollar - ctx->start + 1);
		ctx->new_str = realloc(ctx->new_str, ctx->total_size);
		if (!ctx->new_str)
			return (0);  // Return 0 to indicate failure
		return (1);  // Return 1 to indicate success
	}
	return (0);  // No backslash handling needed
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
	return (1);  // Return 1 to indicate success
}
int	pv_update_pointers(t_context *ctx)
{
	if (ctx->end_of_var)
	{
		*(ctx->end_of_var) = ctx->temp_char;
		ctx->start = ctx->end_of_var;  // Update start if end_of_var is present
	}
	else
		ctx->start = ctx->var_name + strlen(ctx->var_name);  // Otherwise, move start past the variable name
	return (1);  // Return 1 to indicate success
}

int	pv_env_variable(t_context *ctx, char *env_value)
{
	if (env_value == NULL)
		env_value = "";  // Use empty string if no value is found
	ctx->total_size += strlen(env_value);  // Update total size based on the length of env_value
	pv_resize_concat(&(ctx->new_str), ctx->total_size, env_value, (size_t)-1);  // Concatenate env_value to new_str
	return (1);  // Return 1 to indicate success
}

void	pv_fill_values(t_context *ctx)
{
	ctx->total_size += (ctx->dollar - ctx->start);
	ctx->var_name = ctx->dollar + 1;
	ctx->end_of_var = strpbrk(ctx->var_name, " /'\\.#$()?1234567890+\"");
	if (ctx->end_of_var && isdigit(*(ctx->end_of_var))) {
	ctx->end_of_var = strpbrk(ctx->end_of_var, " '\\.#$()?+\"");
	}

	// size_t i = 0;
	// while (ctx->var_name[i] != '\0' && (isalnum(ctx->var_name[i]) || ctx->var_name[i] == '_')) {
	//     i++;
	// }
	// ctx->end_of_var = &ctx->var_name[i];

	ctx->temp_char = '\0';
	ctx->first_char = *(ctx->var_name);
		//printf("Last character reached: '%c'\n", *(ctx->end_of_var - 1));
}

int	pv_handling_0(t_context *ctx)
{
	if (ctx->dollar)
	{
		ctx->total_size += strlen(ctx->dollar);
		pv_resize_concat(&(ctx->new_str), ctx->total_size, ctx->dollar,  strlen(ctx->dollar));
		ctx->start = ctx->end_of_var + 1;
		if (*(ctx->start) == '\0')
			return (1);
	}
	else
	{
		pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + strlen(ctx->dollar) + 1, ctx->dollar, (size_t)-1);
		return (1);
	}
	return (0);
}

void	pv_handling_1(t_context *ctx)
{
	if (ctx->start[ctx->dollar - ctx->start - 1] == '\'') 
		pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + ctx->dollar - ctx->start, ctx->start, ctx->dollar - ctx->start - 1);
	else
		pv_resize_concat(&(ctx->new_str),strlen(ctx->new_str) + ctx->dollar - ctx->start + 1, ctx->start, ctx->dollar - ctx->start);
}

void	pv_handling_2(t_context *ctx)
{
	ctx->start = ctx->dollar + 1;
	ctx->dollar = strchr(ctx->start, '$');
}

void	pv_handling_3(t_context *ctx)
{
	ctx->temp_char = *(ctx->end_of_var);
	*(ctx->end_of_var) = '\0';
}

//export hi$USER'$USER'"$USER"$USER\\$USER\$USER$HOME9.
//export hi$USER'$USER'"$USER"$USER\\$USER\$USER$HOME9$.

char	*handle_env_value(t_context *ctx, t_env *env)
{
	ctx->env_value = get_env_value(env, ctx->var_name);
	if (ctx->env_value == NULL)
		ctx->env_value = "";

	ctx->total_size += strlen(ctx->env_value);
	pv_resize_concat(&(ctx->new_str), strlen(ctx->new_str) + strlen(ctx->env_value) + 1, ctx->env_value, (size_t)-1);

	if (ctx->end_of_var) {
		*(ctx->end_of_var) = ctx->temp_char;
		ctx->start = ctx->end_of_var;
	} else {
		ctx->start = ctx->var_name + strlen(ctx->var_name);
	}

	if (ctx->env_value != NULL && *(ctx->env_value) != '\0') {
		free(ctx->env_value);
	}
	return (ctx->start);
}

int	initialize_context(t_context *ctx, char *input)
{
	if (input == NULL)
		return (0);
	ctx->input = input;
	return (1);
}

void	handle_variable_cases(t_context *ctx, t_env *env)
{
	if (ctx->end_of_var)
	{
		pv_handling_3(ctx);
	}
	if (ctx->first_char == '?')
	{
		if (!pv_question_mark(ctx))
			return ;
	}
	else if ((ctx->dollar > ctx->input && *(ctx->dollar - 1) == '\'' && ctx->temp_char == '\''))
	{
		if (pv_handling_0(ctx))
			return ;
	}
	else if (isdigit(ctx->first_char))
		ctx->start = ctx->var_name + 1;
	else
	{
		ctx->start = handle_env_value(ctx, env);
	}
	ctx->dollar = strchr(ctx->start, '$');
	if (ctx->dollar != NULL && *(ctx->dollar + 1) == '\0')
		ctx->dollar = NULL;
}

void	process_dollar_signs(t_context *ctx, t_env *env)
{
	while (ctx->dollar)
	{
		if (pv_backslashes_cases(ctx))
		{
			pv_handling_2(ctx);
			continue ;
		}
		if (ctx->dollar != ctx->input)
			pv_handling_1(ctx);
		pv_fill_values(ctx);
		handle_variable_cases(ctx, env);
	}
}

char	*process_variable(char *input, t_env *env)
{
	t_context	ctx;

	if (!initialize_context(&ctx, input))
		return (NULL);
	if (!strchr(ctx.input, '$'))
		return (remove_paired_quotes(ft_escape_char(strdup(input))));
	ctx.total_size = pv_initialise_vars(&ctx);
	process_dollar_signs(&ctx, env);
	pv_resize_concat(&ctx.new_str, strlen(ctx.new_str) + strlen(ctx.start) + 1, ctx.start, (size_t)-1);
	return (remove_paired_quotes(ft_escape_char(ctx.new_str)));
}
