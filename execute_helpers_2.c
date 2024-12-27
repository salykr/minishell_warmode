/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:47:28 by marvin            #+#    #+#             */
/*   Updated: 2024/11/24 22:47:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"

void buitlin(t_parser *parser, t_env *env)
{
	if (strcmp(parser->command, "echo") == 0)
		global_var = builtin_echo(parser, env);
	else if (strcmp(parser->command, "env") == 0)
		global_var = builtin_env(parser, env);
	else if (strcmp(parser->command, "pwd") == 0)
		global_var = builtin_pwd(parser, env);
	else if (strcmp(parser->command, "export") == 0)
		global_var = builtin_export(parser, env);
	else if (strcmp(parser->command, "unset") == 0 && parser->next == NULL)
		builtin_unset(parser, env);
	else if (strcmp(parser->command, "cd") == 0 && parser->next == NULL)
		global_var = builtin_cd(parser, env);
	else if (strcmp(parser->command, "exit") == 0)
		builtin_exit(parser, env);
}