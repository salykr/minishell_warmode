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

#include "mini_shell.h"

void	pv_handling_1(t_context *ctx)
{
	if (ctx->start[ctx->dollar - ctx->start - 1] == '\'')
		pv_resize_concat(&(ctx->new_str),
			ft_strlen(ctx->new_str) + ctx->dollar - ctx->start,
			ctx->start, ctx->dollar - ctx->start - 1);
	else
		pv_resize_concat(&(ctx->new_str),
			ft_strlen(ctx->new_str) + ctx->dollar - ctx->start + 1,
			ctx->start, ctx->dollar - ctx->start);
}

void	pv_handling_2(t_context *ctx)
{
	ctx->start = ctx->dollar + 1;
	ctx->dollar = ft_strchr(ctx->start, '$');
}

void	pv_handling_3(t_context *ctx)
{
	ctx->temp_char = *(ctx->end_of_var);
	*(ctx->end_of_var) = '\0';
}

char	*handle_env_value(t_context *ctx, t_env *env)
{
	ctx->env_value = get_env_value(env, ctx->var_name);
	if (ctx->env_value == NULL)
		ctx->env_value = "";
	ctx->total_size += ft_strlen(ctx->env_value);
	pv_resize_concat(&(ctx->new_str),
		ft_strlen(ctx->new_str) + ft_strlen(ctx->env_value) + 1,
		ctx->env_value, (size_t)-1);
	if (ctx->end_of_var)
	{
		*(ctx->end_of_var) = ctx->temp_char;
		ctx->start = ctx->end_of_var;
	}
	else
		ctx->start = ctx->var_name + ft_strlen(ctx->var_name);
	if (ctx->env_value != NULL && *(ctx->env_value) != '\0')
		free(ctx->env_value);
	return (ctx->start);
}

int	initialize_context(t_context *ctx, char *input)
{
	if (input == NULL)
		return (0);
	ctx->input = input;
	return (1);
}
