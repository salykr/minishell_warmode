/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:37:52 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/19 11:59:49 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	deal_with_esc_char(t_parser *list)
{
	int	j;

	j = -1;
	while (list->input[++j])
	{
		if (ft_strchr(list->input[j], '\\'))
			list->input[j] = ft_escape_char(list->input[j]);
	}
}

int	deal_with_name_no_var(t_parser *list, t_env *env)
{
	char	*input_copy;
	char	*new_name;

	if (ft_strchr(list->input[0], '$') != NULL
		&& ft_strchr(list->input[0], '=') == NULL)
	{
		input_copy = ft_strdup(list->input[0]);
		new_name = process_variable(input_copy, env);
		if (!new_name || *new_name == '\0')
			return (print_env_sorted(env),
				handle_memory_errors(input_copy, new_name), 0);
		if (check_input(new_name) == 0 || check_input(new_name) == -1)
			return (handle_memory_errors(input_copy, new_name), 1);
		return (handle_memory_errors(input_copy, new_name), -1);
	}
	return (-1);
}

void	deal_with_quotes(t_parser *list, int i)
{
	char	*first_quote;
	char	*equals_sign;

	first_quote = ft_strchr(list->input[i], '"');
	if (first_quote)
	{
		equals_sign = ft_strrchr(first_quote + 1, '=');
		if (equals_sign && *(equals_sign - 1) == '"')
		{
			list->input[i] = remove_closing_quote_after_equals(list->input[i]);
		}
	}
}

int	handle_export_input(char *input, t_env *env, int *return_value)
{
	char	*name;
	char	*value;
	int		nb;

	parse_export_input(input, &name, &value);
	if (name && check_input(name) == 0)
	{
		handle_memory_errors(name, value);
		*return_value = 1;
		return (0);
	}
	if ((name && ft_strchr(name, ';')) || (value && ft_strchr(value, ';')))
	{
		*return_value = add_or_update_to_env(name, value, env);
		return (1);
	}
	if (name)
	{
		nb = add_or_update_to_env(name, value, env);
		if (*return_value == 0)
			*return_value = nb;
	}
	return (0);
}

int	builtin_export(t_parser *list, t_env *env)
{
	int	i;
	int	return_value;
	int	nb;

	i = 0;
	return_value = 0;
	if (list->operations != NULL)
		return ((void)printf("invalid option.\n"), 2);
	if (!list->input || !list->input[0])
		return (print_env_sorted(env), 0);
	deal_with_esc_char(list);
	nb = deal_with_name_no_var(list, env);
	if (nb != -1)
		return (nb);
	while (list->input[i])
	{
		deal_with_quotes(list, i);
		if (handle_export_input(list->input[i++], env, &return_value))
			break ;
	}
	return (return_value);
}
