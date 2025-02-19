/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:29:23 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 15:29:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	is_matching_env_var(char *env_var, char *new_name)
{
	size_t	name_len;

	name_len = ft_strlen(new_name);
	return (ft_strncmp(env_var, new_name, name_len) == 0
		&& (env_var[name_len] == '=' || env_var[name_len] == '\0'));
}

int	update_existing_value(char **env_var, char *new_name,
		char *new_value, char *current_value)
{
	char	*updated_value;

	current_value++;
	updated_value = concatenate_value(current_value, new_value);
	if (!updated_value)
		return (1);
	if (*env_var != NULL)
		free(*env_var);
	*env_var = malloc(ft_strlen(new_name) + ft_strlen(updated_value) + 2);
	if (!(*env_var))
	{
		free(updated_value);
		return (1);
	}
	ft_strcpy(*env_var, new_name);
	ft_strcat(*env_var, "=");
	ft_strcat(*env_var, updated_value);
	free(updated_value);
	return (1);
}

int	handle_input_status_negative(char **env_var,
		char *new_name, char *new_value)
{
	char	*current_value;

	current_value = ft_strchr(*env_var, '=');
	if (current_value)
	{
		update_existing_value(env_var, new_name, new_value, current_value);
		handle_memory_errors(new_name, new_value);
		return (1);
	}
	replace_or_append_value(env_var, new_name, new_value);
	handle_memory_errors(new_name, new_value);
	return (1);
}

int	find_and_update_env(int check_input_status, char *new_name,
		char *new_value, t_env *env)
{
	int	i;

	i = 0;
	while (env->env[i] != NULL)
	{
		if (is_matching_env_var(env->env[i], new_name))
		{
			if (check_input_status == -1)
				return (handle_input_status_negative(&env->env[i],
						new_name, new_value));
			replace_or_append_value(&env->env[i], new_name, new_value);
			if (ft_strcmp("SHLVL", new_name) != 0)
				handle_memory_errors(new_name, new_value);
			return (1);
		}
		i++;
	}
	return (0);
}
