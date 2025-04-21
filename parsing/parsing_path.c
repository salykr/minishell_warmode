/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:26:47 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 16:26:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_executable(char *cmd, t_env env)
{
	char	*path;
	char	*path_copy;
	char	*dir;
	char	full_path[1024];

	path = retreive_path(env);
	if (path == NULL)
		return (0);
	path_copy = ft_strdup(path);
	dir = ft_strtok(path_copy, ":");
	while (dir != NULL)
	{
		ft_strcpy(full_path, dir);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (1);
		}
		dir = ft_strtok(NULL, ":");
	}
	free(path_copy);
	return (0);
}

int	is_executable_pwd(t_env env, char *cmd)
{
	char	*cmd_1;

	cmd_1 = get_path_pwd(&env, cmd);
	if (cmd_1 != NULL && access(cmd_1, X_OK) == 0)
	{
		free(cmd_1);
		return (1);
	}
	if (cmd_1 != NULL)
		free(cmd_1);
	return (0);
}

int	handle_parsing_path_helper_1(t_input *tokens, t_parser *curr, t_env *env)
{
	struct stat	path_stat;

	if (curr->command == NULL && (is_executable(tokens->value, *env)
			|| is_executable_pwd(*env, tokens->value)))
	{
		curr->command = ft_strdup(tokens->value);
		if (curr->command == NULL)
		{
			perror("Failed to allocate memory");
			return (-1);
		}
	}
	else if ((curr->command != NULL && cmd_is_dir(tokens->value))
		|| stat(tokens->value, &path_stat) == 0)
		add_to_input_args(tokens->value, curr);
	else
	{
		ft_putendl_fd("bash: No such file or directory", 2);
		env->exit_code = 127;
		return (g_v = 0, -1);
	}
	return (0);
}

int	handle_parsing_path_helper_2(t_input *tokens, t_parser *curr, t_env *env)
{
	char	*p;

	p = ft_strrchr(tokens->value, '/');
	if (p != NULL)
		p++;
	if (*p != '\0')
	{
		if (is_executable(p, *env) || is_builtin_command(tokens->value)
			|| access(tokens->value, X_OK) == 0)
		{
			curr->command = ft_strdup(tokens->value);
			curr->args = add_string_to_2d_array(curr->args, tokens->value);
			if (curr->command == NULL)
				return (perror("Failed to allocate memory"), -1);
		}
		else if (access(tokens->value, X_OK) != 0)
			return (env->exit_code = 127, g_v = 0, errmsg_cmd(tokens->value,
					NULL, "No such file or directory"), -1);
		else
			add_to_input_args(tokens->value, curr);
	}
	else
		add_to_input_args(tokens->value, curr);
	return (0);
}

int	handle_parsing_path(t_input *tokens, t_parser *curr, t_env *env)
{
	if (tokens->type == T_PATH)
	{
		if (curr->command != NULL && !strcmp(curr->command, "pwd"))
			return (1);
		if (curr->command == NULL && cmd_is_dir(tokens->value))
			return (env->exit_code = 126, g_v = 0, errmsg_cmd(tokens->value,
					NULL, "Is a directory"), 1);
		else if (!ft_strncmp(tokens->value, "./", 2))
		{
			if (handle_parsing_path_helper_1(tokens, curr, env) < 0)
				return (-1);
		}
		else if (!ft_strncmp(tokens->value, "/", 1)
			|| !ft_strncmp(tokens->value, "~/", 2))
		{
			if (handle_parsing_path_helper_2(tokens, curr, env) < 0)
				return (-1);
		}
		else if (!cmd_is_dir(tokens->value))
			return (printf("bash: %s : No such file or directory\n",
					tokens->value), env->exit_code = 1, g_v = 0, -1);
	}
	return (0);
}
