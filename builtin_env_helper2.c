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
		if (!strncmp(myenv->env[i], str, len) && myenv->env[i][len] == '=')
			return (myenv->env[i] + len + 1);
		i++;
	}
	return (NULL);
}


t_env *init_env(char **env)
{
    t_env *myenv;
    int i;
    
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
    printf("(((((i is %d))))))\n",i);
    update_SHLVL(myenv);
    return (myenv);
}
