/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:52:20 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/19 12:49:25 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	process_dollar_strings(char **strs, t_env *env)
{
	char	*processed_str;
	int		i;

	i = 0;
	if (strs == NULL)
		return ;
	while (strs[i] != NULL)
	{
		if (ft_strchr(strs[i], '$') != NULL)
		{
			processed_str = process_variable(strs[i], env);
			free(strs[i]);
			strs[i] = processed_str;
		}
		i++;
	}
}

char	*retreive_path(t_env env)
{
	if (ft_getenv(&env, "SHLVL") == NULL)
		return (NULL);
	else if (ft_strcmp(ft_getenv(&env, "SHLVL"), "1") == 0)
	{
		return (ft_strdup(
				"/home/isalayan/bin:"
				"/usr/local/sbin:"
				"/usr/local/bin:"
				"/usr/sbin:"
				"/usr/bin:"
				"/sbin:"
				"/bin:"
				"/usr/games:"
				"/usr/local/games:"
				"/snap/bin"
			));
	}
	else
		return (ft_getenv(&env, "PATH"));
}

void	process_files(char **files, t_env *env)
{
	int		i;
	char	*file;
	char	*filee;

	if (!files)
		return ;
	i = -1;
	while (files[++i])
	{
		file = add_path(files[i]);
		filee = remove_paired_quotes(file);
		files[i] = process_variable(filee, env);
		free(filee);
	}
}

void	process_parser_files(t_parser *parser, t_env *env)
{
	t_parser	*current;

	current = parser;
	while (current)
	{
		if (current->infile)
			process_files(current->infile, env);
		if (current->outfile)
			process_files(current->outfile, env);
		current = current->next;
	}
}

int	get_variable(t_parser *parser, t_env *env)
{
	char	*variable;
	int		val;

	val = 0;
	variable = retreive_path(*env);
	if (variable == NULL && ft_strcmp(parser->command, "env") == 0)
	{
		ft_putendl_fd("bash: env: No such file or directory", 2);
		val = 1;
	}
	if (variable != NULL && ft_strcmp(ft_getenv(env, "SHLVL"), "1") == 0)
		free(variable);
	return (val);
}
