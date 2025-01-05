/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:06:34 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 17:06:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	handle_parsing_quotes_helper_2(t_input *tokens, char *value)
{
	if (!strncmp(tokens->value, "$?", 2))
	{
		if (!strcmp(tokens->value, "$?"))
		{
			printf("bash:%d: command not found\n", g_v);
			g_v = 127;
			return (-1);
		}
		else
		{
			printf("bash:%d%d: command not found\n", g_v, g_v);
			g_v = 127;
			return (-1);
		}
	}
	else
	{
		printf("command %s not found\n", value);
		g_v = 127;
		return (-1);
	}
}

int	handle_parsing_quotes_helper(t_input *tokens, t_parser *curr, char *value)
{
	if (curr->command != NULL && (!ft_strcmp(curr->command, "echo")
			|| !ft_strcmp(curr->command, "export")))
		add_to_input_args(tokens->value, curr);
	else if (curr->command == NULL && cmd_is_dir(value))
	{
		errmsg_cmd(value, NULL, "Is a directory");
		free(value);
		g_v = 0;
		return (-1);
	}
	else if (curr->command == NULL)
	{
		handle_parsing_quotes_helper_2(tokens, value);
		free(value);
		return (-1);
	}
	else
	{
		curr->input = add_string_to_2d_array(curr->input, value);
		curr->args = add_string_to_2d_array(curr->args, value);
	}
	return (0);
}

int	handle_parsing_quotes_main_helper(t_input *tokens, t_parser *curr,
		t_env env, char *value)
{
	if ((tokens->value[0] == '\'' && tokens->value[1] == '\''
			&& !tokens->value[2]) || (tokens->value[0] == '\"'
			&& tokens->value[1] == '\"' && !tokens->value[2]))
		curr->input = add_string_to_2d_array(curr->input, tokens->value);
	else if (curr->command == NULL && (is_executable(value, env)
			|| ft_strcmp(value, "cd") == 0 || ft_strcmp(value, "exit") == 0
			|| ft_strcmp(value, "export") == 0 || ft_strcmp(value,
				"unset") == 0))
	{
		curr->command = value;
		curr->args = add_string_to_2d_array(curr->args, value);
		return (0);
	}
	else if (ft_check_n_operation(tokens->value) == 0)
		curr->operations = add_string_to_2d_array(curr->operations,
				tokens->value);
	else if (handle_parsing_quotes_helper(tokens, curr, value) == -1)
		return (-1);
	free(value);
	return (0);
}

int	handle_parsing_quotes(t_input *tokens, t_parser *curr, t_env env)
{
	char	*value;
	char	*temp_value;

	value = NULL;
	temp_value = NULL;
	temp_value = remove_quotes(tokens->value);
	if (temp_value == NULL)
		return (-1);
	if (curr->command == NULL)
		value = process_variable(temp_value, &env);
	else
		value = ft_strdup(temp_value);
	free(temp_value);
	if (value == NULL || value[0] == '\0')
		return (1);
	else if (handle_parsing_quotes_main_helper(tokens, curr, env, value) == -1)
		return (-1);
	return (0);
}
