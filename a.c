/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:09:23 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 16:09:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	process_name_and_value(char *name, char *value,
		t_env *env, char **new_name, char **new_value)
{
	int	check_input_status;

	*new_name = process_variable (name, env);
	if (value != NULL)
		*new_value = process_variable(value, env);
	else
		*new_value = NULL;
	if (!(*new_name) || (value && !(*new_value)))
	{
		handle_memory_errors(*new_name, *new_value);
		return (0);
	}
	check_semicolon(*new_name, new_value);
	check_input_status = check_input(*new_name);
	if (check_input_status == 0 || (value != NULL
			&& *new_value != NULL && !check_value(*new_value)))
	{
		handle_memory_errors(*new_name, *new_value);
		return (0);
	}
	if (check_input_status == -1 && (*new_name)[strlen(*new_name) - 1] == '+')
		(*new_name)[strlen(*new_name) - 1] = '\0';
	return (check_input_status);
}
