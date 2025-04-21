/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_identifier                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:26:42 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 16:26:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	handle_parsing_identifier_helper_errors_helper(t_input *tokens,
		t_parser *curr, t_env *env)
{
	if (curr->command == NULL && ft_strchr(tokens->value, '/'))
	{
		if (cmd_is_dir(tokens->value))
		{
			errmsg_cmd(tokens->value, NULL, "Is a directory");
			env->exit_code = 126;
			return (g_v = 0, 1);
		}
		else
		{
			printf("bash: %s : No such file or directory\n", tokens->value);
			env->exit_code = 127;
			return (g_v = 0, -1);
		}
	}
	else if (tokens->value && !check_balanced_quotes(tokens->value))
	{
		printf("Error: Unbalanced quotes in argument\n");
		env->exit_code = 1;
		return (g_v = 0, -1);
	}
	return (0);
}

int	handle_parsing_identifier_helper_errors(t_input *tokens, t_parser *curr,
			t_env *env)
{
	int	value;

	if (!ft_strncmp(tokens->value, "\\n", 2))
	{
		if (curr->command == NULL)
		{
			ft_putendl_fd("minishell: command not found", 2);
			env->exit_code = 127;
			return (g_v = 0, -1);
		}
	}
	else if (!ft_strcmp(tokens->value, ":") || !ft_strcmp(tokens->value, "!"))
	{
		env->exit_code = 127;
		return (g_v = 0, -1);
	}
	else if (curr->command == NULL && !ft_strncmp(tokens->value, "&&", 2))
	{
		printf("bash: syntax error near unexpected token `&&' \n");
		env->exit_code = 2;
		return (g_v = 0, -1);
	}
	value = handle_parsing_identifier_helper_errors_helper(tokens, curr, env);
	return (value);
}

int	handle_parsing_identifier_helper(t_input *tokens, t_parser *curr,
		char *value, t_env *env)
{
	if (curr->command == NULL || !tokens->value[0])
	{
		if (is_all_spaces(tokens->value))
		{
			free(value);
			return (-1);
		}
		ft_putendl_fd("minishell: command not found", 2);
		env->exit_code = 127;
		free(value);
		return (g_v = 0, -1);
	}
	else
		add_to_input_args(tokens->value, curr);
	free(value);
	return (0);
}

int	handle_parsing_identifier_main(t_input *tokens, t_parser *curr, t_env *env,
		char *value)
{
	char	**split_value;
	int		i;

	i = 0;
	if (curr->command == NULL && ft_strchr(value, ' ')
		&& ft_strchr(tokens->value, '$'))
	{
		split_value = ft_split(value, ' ');
		curr->command = ft_strdup(split_value[0]);
		curr->args = add_string_to_2d_array(curr->args, split_value[0]);
		while (split_value[++i])
			add_to_input_args(split_value[i], curr);
		return (free_2d_array(split_value), free(value), 0);
	}
	if ((is_executable(value, *env) || ft_strcmp(value, "cd") == 0
			|| ft_strcmp(value, "exit") == 0 || ft_strcmp(value, "export") == 0
			|| ft_strcmp(value, "unset") == 0 || ft_strcmp(value, "echo") == 0
			||ft_strcmp(value, "env") == 0 || ft_strcmp(value, "pwd") == 0)
		&& (curr->command == NULL && !is_all_spaces(value)))
	{
		curr->command = value;
		curr->args = add_string_to_2d_array(curr->args, value);
		return (0);
	}
	return (1);
}

int	handle_parsing_identifier(t_input *tokens, t_parser *curr, t_env *env)
{
	char	*value;
	char	*temp_value;
	int		return_value;

	value = NULL;
	temp_value = NULL;
	return_value = handle_parsing_identifier_helper_errors(tokens, curr, env);
	if (return_value == 1 || return_value == -1)
		return (return_value);
	temp_value = remove_quotes(tokens->value);
	if (curr->command == NULL)
		value = process_variable(temp_value, env);
	else
		value = ft_strdup(temp_value);
	free(temp_value);
	if (value == NULL || value[0] == '\0')
		return (1);
	else if (handle_parsing_identifier_main(tokens, curr, env, value) == 0)
		return (0);
	else if (handle_parsing_identifier_helper(tokens, curr, value, env) < 0)
		return (-1);
	else if (curr->command == NULL)
		free(value);
	return (0);
}
