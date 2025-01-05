/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_helper2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:35:49 by marvin            #+#    #+#             */
/*   Updated: 2024/11/12 18:35:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*ft_getenv(t_env *myenv, char *str)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	while (myenv->env[i])
	{
		if (!ft_strncmp(myenv->env[i], str, len) && myenv->env[i][len] == '=')
			return (myenv->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

t_env	*init_env(char **env)
{
	t_env	*myenv;
	int		i;

	i = 0;
	myenv = (t_env *)malloc(sizeof(t_env));
	if (!myenv)
		return (NULL);
	while (env[i])
		i++;
	myenv->env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!myenv->env)
	{
		free(myenv);
		return (NULL);
	}
	i = 0;
	while (env[i])
	{
		myenv->env[i] = ft_strdup(env[i]);
		i++;
	}
	myenv->env[i] = NULL;
	// printf("(((((i is %d))))))\n", i);
	update_shlvl(myenv);
	return (myenv);
}

void	ft_free_env(t_env **my_env)
{
	int	i;

	i = 0;
	if (!my_env || !(*my_env))
		return ;
	while ((*my_env)->env[i])
	{
		free((*my_env)->env[i]);
		i++;
	}
	free((*my_env)->env);
	free(*my_env);
	*my_env = NULL;
}

t_env	*manage_env_i(void)
{
	t_env	*myenv;
	char	*wd;
	char	cwd[2048];

	myenv = (t_env *)malloc(sizeof(t_env));
	if (!myenv)
		return (NULL);
	myenv->env = (char **)malloc(sizeof(char *) * (4));
	if (!myenv->env)
	{
		free(myenv);
		return (NULL);
	}
	wd = ft_strdup("PWD=");
	myenv->env[0] = ft_strdup("SHLVL=1");
	myenv->env[1] = ft_str_join(wd, getcwd(cwd, sizeof(cwd)));
	free(wd);
	myenv->env[2] = ft_strdup("_=/usr/bin/env");
	myenv->env[3] = NULL;
	return (myenv);
}
