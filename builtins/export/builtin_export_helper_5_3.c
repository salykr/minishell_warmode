/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_5_3.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:18:23 by marvin            #+#    #+#             */
/*   Updated: 2025/01/06 19:49:43 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	handle_variable_cases(t_context *ctx, t_env *env)
{
	if (ctx->end_of_var)
		pv_handling_3(ctx);
	if (ctx->first_char == '?')
	{
		if (!pv_question_mark(ctx))
			return ;
	}
	else if ((ctx->dollar > ctx->input && *(ctx->dollar - 1) == '\''
			&& ctx->temp_char == '\''))
	{
		if (pv_handling_0(ctx))
			return ;
	}
	else if (ft_isdigit(ctx->first_char))
		ctx->start = ctx->var_name + 1;
	else
		ctx->start = handle_env_value(ctx, env);
	ctx->dollar = ft_strchr(ctx->start, '$');
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
	char		*result;

	if (!initialize_context(&ctx, input))
		return (NULL);
	if (ft_strchr(ctx.input, '$') == NULL)
		return (remove_paired_quotes(ft_escape_char(ft_strdup(input))));
	ctx.total_size = pv_initialise_vars(&ctx);
	process_dollar_signs(&ctx, env);
	pv_resize_concat(&ctx.new_str, ft_strlen(ctx.new_str)
		+ ft_strlen(ctx.start) + 1, ctx.start, (size_t)-1);
	result = remove_paired_quotes(ft_escape_char(ctx.new_str));
	if (result != NULL && ft_strlen(result) == 0)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

// export hi$USER'$USER'"$USER"$USER\\$USER\$USER$HOME9.
// export hi$USER'$USER'"$USER"$USER\\$USER\$USER$HOME9$.