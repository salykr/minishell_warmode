/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:14:15 by marvin            #+#    #+#             */
/*   Updated: 2025/01/02 16:45:20 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*concatenate_value(char *current_value, char *new_value)
{
	size_t	current_len;
	size_t	new_len;
	char	*updated_value;

	current_len = ft_strlen(current_value);
	if (new_value)
		new_len = current_len + ft_strlen(new_value);
	else
		new_len = current_len;
	updated_value = malloc(new_len + 1);
	if (!updated_value)
		return (NULL);
	ft_strcpy(updated_value, current_value);
	if (new_value)
		ft_strcat(updated_value, new_value);
	return (updated_value);
}

void	replace_or_append_value(char **env_entry,
		char *new_name, char *new_value)
{
	size_t	name_len;
	size_t	new_len;

	name_len = ft_strlen(new_name);
	if (new_value)
		new_len = name_len + ft_strlen(new_value) + 1;
	else
		new_len = name_len + 1;
	if (*env_entry != NULL)
		free(*env_entry);
	*env_entry = malloc(new_len + 1);
	if (!*env_entry)
		return ;
	ft_strcpy(*env_entry, new_name);
	if (new_value)
	{
		ft_strcat(*env_entry, "=");
		ft_strcat(*env_entry, new_value);
	}
	else
		ft_strcat(*env_entry, "");
}

void	parse_export_input(char *input, char **name, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(input, '=');
	*name = NULL;
	*value = NULL;
	if (equal_sign)
	{
		*name = ft_strndup(input, equal_sign - input);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*name = ft_strdup(input);
		*value = NULL;
	}
}

int	process_name_and_value(char *name, char *value,
		t_env *env, t_name_value *new_nv)
{
	int	check_input_status;

	new_nv->new_name = process_variable(name, env);
	if (value != NULL)
	{
		new_nv->new_value = process_variable(value, env);
		if (new_nv->new_value == NULL)
			new_nv->new_value = ft_strdup("");
	}	
	else
		new_nv->new_value = NULL;
	handle_memory_errors(name, value);
	if (!(new_nv->new_name) || (value && !(new_nv->new_value)))
		return (handle_memory_errors(new_nv->new_name, new_nv->new_value), 0);
	check_semicolon(new_nv->new_name, &(new_nv->new_value));
	check_input_status = check_input_end(new_nv->new_name);
	if (check_input_status == 0 || (value != NULL && new_nv->new_value != NULL
			&& !check_value(new_nv->new_value)))
		return (handle_memory_errors(new_nv->new_name, new_nv->new_value), 0);
	if ((new_nv->new_name)[ft_strlen(new_nv->new_name) - 1] == '+')
		(new_nv->new_name)[ft_strlen(new_nv->new_name) - 1] = '\0';
	return (check_input_status);
}

int	add_or_update_to_env(char *name, char *value, t_env *env)
{
	t_name_value	new_nv;
	char			**new_env;
	int				check_input_status;
	int				i;

	new_nv.new_name = NULL;
	new_nv.new_value = NULL;
	check_input_status = handle_shlvl_case(name, value, &new_nv);
	if (check_input_status == 2)
		check_input_status = process_name_and_value(name, value, env, &new_nv);
	if (check_input_status == 0)
		return (1);
	if (find_and_update_env(check_input_status,
			new_nv.new_name, new_nv.new_value, env))
		return (0);
	i = ft_doublecharlen(env);
	new_env = realloc(env->env, sizeof(char *) * (i + 2));
	if (!new_env)
		return (handle_memory_errors(new_nv.new_name, new_nv.new_value), 1);
	env->env = new_env;
	replace_or_append_value(&env->env[i], new_nv.new_name, new_nv.new_value);
	env->env[i + 1] = NULL;
	return (handle_memory_errors(new_nv.new_name, new_nv.new_value), 0);
}
