/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_0.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:52:38 by marvin            #+#    #+#             */
/*   Updated: 2025/01/02 16:08:30 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*get_env_value(t_env *env, const char *var)
{
	int		i;
	char	*entry;
	char	*equals;
	size_t	var_len;

	i = 0;
	if (var[0] == '$')
		var++;
	entry = env->env[i];
	while (entry != NULL)
	{
		equals = ft_strchr(entry, '=');
		if (equals)
		{
			var_len = equals - entry;
			if (ft_strncmp(entry, var, var_len) == 0
				&& ft_strlen(var) == var_len)
				return (ft_strdup(equals + 1));
		}
		i++;
		entry = env->env[i];
	}
	return (NULL);
}

static int	find_smallest_unprinted(char **env, int *printed, int count)
{
	int	min_idx;
	int	j;

	min_idx = -1;
	j = 0;
	while (j < count)
	{
		if (!printed[j] && (min_idx == -1
				|| ft_strcmp(env[j], env[min_idx]) < 0))
			min_idx = j;
		j++;
	}
	return (min_idx);
}

static void	print_env_var(char *env_var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(env_var, '=');
	if (env_var && ft_strlen(env_var) == 1 && *env_var == '_')
		return ;
	if (equal_sign)
		printf("declare -x %.*s=\"%s\"\n", (int)(equal_sign - env_var),
			env_var, equal_sign + 1);
	else
		printf("declare -x %s\n", env_var);
}

static int	count_and_initialize(t_env *env, int **printed)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (env->env[count])
		count++;
	*printed = malloc(count * sizeof(int));
	if (!*printed)
		return (-1);
	while (i < count)
	{
		(*printed)[i] = 0;
		i++;
	}
	return (count);
}

void	print_env_sorted(t_env *env)
{
	int	*printed;
	int	count;
	int	printed_count;
	int	min_idx;

	printed_count = 0;
	count = count_and_initialize(env, &printed);
	if (count == -1)
		return ;
	while (printed_count < count)
	{
		min_idx = find_smallest_unprinted(env->env, printed, count);
		print_env_var(env->env[min_idx]);
		printed[min_idx] = 1;
		printed_count++;
	}
	free(printed);
}
