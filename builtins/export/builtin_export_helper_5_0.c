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

char	*resize_string(char *str, size_t new_size)
{
	char	*new_str;

	if (!str || new_size == 0)
		return (NULL);
	new_str = (char *)malloc(new_size);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	ft_strncpy(new_str, str, new_size - 1);
	new_str[new_size - 1] = '\0';
	free(str);
	return (new_str);
}

size_t	pv_count_backslashes(t_context *ctx)
{
	size_t		backslash_count;
	char		*p;

	p = ctx->dollar - 1;
	backslash_count = 0;
	if (ctx->dollar != ctx->input)
	{
		while (p >= ctx->input && *p == '\\')
		{
			backslash_count++;
			p--;
		}
	}
	return (backslash_count);
}

void	pv_resize_concat(char **resized_str, size_t len_resize,
	char *concat_str, size_t len_concat)
{
	*resized_str = resize_string(*resized_str, len_resize);
	if (len_concat == (size_t)-1)
		ft_strcat(*resized_str, concat_str);
	else
		ft_strncat(*resized_str, concat_str, len_concat);
}

int	pv_initialise_vars(t_context *ctx)
{
	if (!ctx->input)
		return (-1);
	ctx->start = ctx->input;
	ctx->dollar = ft_strchr(ctx->input, '$');
	ctx->new_str = malloc(1);
	if (!ctx->new_str)
		return (-1);
	(ctx->new_str)[0] = '\0';
	return (1);
}

int	pv_backslashes_cases(t_context *ctx)
{
	size_t	backslash_count;

	backslash_count = pv_count_backslashes(ctx);
	if (backslash_count % 2 != 0)
	{
		pv_resize_concat(&(ctx->new_str),
			ft_strlen(ctx->new_str) + ctx->dollar - ctx->start + 2,
			ctx->start, ctx->dollar - ctx->start + 1);
		ctx->total_size += (ctx->dollar - ctx->start + 1);
		ctx->new_str = realloc(ctx->new_str, ctx->total_size);
		if (!ctx->new_str)
			return (0);
		return (1);
	}
	return (0);
}
