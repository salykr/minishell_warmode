/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:53:38 by saoun             #+#    #+#             */
/*   Updated: 2025/04/12 12:38:27 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	validate_variable_name(char *name, bool save_val, t_env *env)
{
	if (name && !is_valid_var_name(name))
	{
		if (name && includes_exlamation_mark(name))
		{
			env->exit_code = 0;
			g_v = 0;
			free(name);
			return (1);
		}
		if (!save_val && *name != '\0')
			env->exit_code = 1;
		g_v = 0;
		free(name);
		return (1);
	}
	return (0);
}

void	process_variable_removal(t_env *myenv, char *name)
{
	int	index;

	index = find_env_var(myenv, name);
	if (index != -1)
		remove_env_var(myenv, index);
	free(name);
}

static int	process_unset_input(t_parser *parser, t_env *myenv, bool save_val)
{
	char	*name;
	int		i;

	i = 0;
	while (parser->input && parser->input[i])
	{
		name = process_variable(parser->input[i], myenv);
		if (validate_variable_name(name, save_val, myenv))
			return (myenv->exit_code);
		process_variable_removal(myenv, name);
		i++;
	}
	return (myenv->exit_code);
}

int	builtin_unset(t_parser *parser, t_env *myenv)
{
	bool	save_val;

	save_val = false;
	if (!parser || (!parser->input && !parser->operations))
	{
		printf("\n");
		return (0);
	}
	if (parser->operations)
	{
		if (handle_operations_dash(parser, myenv))
			return (myenv->exit_code);
	}
	if (parser->input && handle_input_dash(parser, &save_val, myenv))
		return (myenv->exit_code);
	return (process_unset_input(parser, myenv, save_val));
}
