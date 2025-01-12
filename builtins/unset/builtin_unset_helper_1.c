/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_helper_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 12:20:46 by skreik            #+#    #+#             */
/*   Updated: 2025/01/12 15:17:44 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	includes_exlamation_mark(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] && str[i] == '!' && str[i + 1] && str[i + 1] != '\0')
		{
			printf("bash: !%c event not found\n", (str[i + 1]));
			return (1);
		}
		i++;
	}
	return (0);
}

int	handle_unset_options(char **operations, bool *save_val)
{
	if (operations[0] && operations[0][0] == '-')
	{
		*save_val = true;
		if (operations[0][1] && operations[0][1] == '-'
			&& operations[0][2] != '\0')
		{
			return (2);
		}
		else if (operations[0][1] && operations[0][1] == '-'
			&& operations[0][2] == '\0')
		{
			printf("\n");
			return (0);
		}
	}
	return (-1);
}

int	handle_operations_dash(t_parser *parser)
{
	if (parser->operations[0] && parser->operations[0][0] == '-')
	{
		printf("-bash: unset: --: invalid option\nunset:");
		printf(" usage: unset [-f] [-v] [-n] [name ...]\n");
		g_v = 2;
		return (1);
	}
	return (0);
}

int	handle_input_dash(t_parser *parser, bool *save_val)
{
	if (parser->input[0] && parser->input[0][0] == '-')
	{
		*save_val = true;
		if (parser->input[0][1] == '\0')
		{
			printf("\n");
			g_v = 0;
		}
		else if (parser->input[0][1] && parser->input[0][1] == '-'
					&& parser->input[0][2] != '\0')
		{
			printf("-bash: unset: --: invalid option\nunset:");
			printf(" usage: unset [-f] [-v] [-n] [name ...]\n");
			g_v = 2;
		}
		else if (parser->input[0][1] && parser->input[0][1] == '-'
					&& parser->input[0][2] == '\0')
		{
			printf("\n");
			g_v = 0;
		}
		return (1);
	}
	return (0);
}
