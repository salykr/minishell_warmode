/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdennaou <rdennaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:50:51 by saoun             #+#    #+#             */
/*   Updated: 2024/12/26 12:36:13 by rdennaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*resize_buffer(char *buffer, size_t *size)
{
	char	*new_buffer;

	*size *= 2;
	new_buffer = (char *)ft_realloc(buffer, *size / 2, *size);
	if (new_buffer == NULL)
	{
		perror("ft_realloc");
		free(buffer);
		exit(EXIT_FAILURE);
	}
	return (new_buffer);
}

void	handle_pwd_env(t_env *env)
{
	char	*pwd_env;
	char	*path;

	pwd_env = ft_getenv(env, "PWD");
	if (pwd_env != NULL)
	{
		path = strstr(pwd_env, "PATH=");
		if (path != NULL)
			printf("%s\n", path + 5);
		else
			printf("%s\n", pwd_env);
	}
	else
		perror("getcwd and PWD not set");
}

int	builtin_pwd(t_parser *parser, t_env *env)
{
	size_t	size;
	char	*buffer;

	size = INITIAL_SIZE;
	if (parser->operations != NULL || (parser->input != NULL
			&& ft_strlen(parser->input[0]) > 2 && parser->input[0][2] == '-'))
		return (printf("bash: pwd: invalid option\n"), 2);
	buffer = (char *)malloc(size);
	if (buffer == NULL)
		return (1);
	while (getcwd(buffer, size) == NULL)
	{
		if (errno == ERANGE)
			buffer = resize_buffer(buffer, &size);
		else
		{
			handle_pwd_env(env);
			return (free(buffer), 1);
		}
	}
	printf("%s\n", buffer);
	free(buffer);
	return (0);
}
