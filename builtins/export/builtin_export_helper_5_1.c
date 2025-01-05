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

#include "mini_shell.h"

int	pv_question_mark(t_context *ctx)
{
	char	*exit_status;

	exit_status = ft_itoa(g_v);
	if (!exit_status)
		return (0);
	ctx->total_size += strlen(exit_status);
	pv_resize_concat(&(ctx->new_str), ctx->total_size, exit_status, (size_t)-1);
	free(exit_status);
	if (ctx->end_of_var)
		ctx->start = ctx->end_of_var + 1;
	else
		return (0);
	return (1);
}

int	pv_update_pointers(t_context *ctx)
{
	if (ctx->end_of_var)
	{
		*(ctx->end_of_var) = ctx->temp_char;
		ctx->start = ctx->end_of_var;
	}
	else
		ctx->start = ctx->var_name + strlen(ctx->var_name);
	return (1);
}

int	pv_env_variable(t_context *ctx, char *env_value)
{
	if (env_value == NULL)
		env_value = "";
	ctx->total_size += strlen(env_value);
	pv_resize_concat(&(ctx->new_str), ctx->total_size, env_value, (size_t)-1);
	return (1);
}

void	pv_fill_values(t_context *ctx)
{
	ctx->total_size += (ctx->dollar - ctx->start);
	ctx->var_name = ctx->dollar + 1;
	ctx->end_of_var = strpbrk(ctx->var_name, " '\\/.#$()?1234567890+\"");
	if (ctx->end_of_var && isdigit(*(ctx->end_of_var)))
		ctx->end_of_var = strpbrk(ctx->end_of_var, " '\\.#$()?+\"");
	ctx->first_char = *(ctx->var_name);
}

int	pv_handling_0(t_context *ctx)
{
	if (ctx->dollar)
	{
		ctx->total_size += strlen(ctx->dollar);
		pv_resize_concat(&(ctx->new_str), ctx->total_size,
			ctx->dollar, strlen(ctx->dollar));
		ctx->start = ctx->end_of_var + 1;
		if (*(ctx->start) == '\0')
			return (1);
	}
	else
	{
		pv_resize_concat(&(ctx->new_str),
			strlen(ctx->new_str) + strlen(ctx->dollar) + 1,
			ctx->dollar, (size_t) - 1);
		return (1);
	}
	return (0);
}
