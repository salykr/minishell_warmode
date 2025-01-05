/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_UTILS.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:27:03 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 16:27:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

bool	ft_check_n_operation(char *str)
{
	int	i;

	i = 1;
	if (str[0] == '\'' || str[0] == '\"')
		str = remove_quotes(str);
	while (str[i])
	{
		if (str[i] != 'n')
			return (free(str), 1);
		i++;
	}
	free(str);
	return (0);
}

int	count_rows(char **array)
{
	int	count;

	count = 0;
	if (!array)
		return (0);
	while (array[count] != NULL)
		count++;
	return (count);
}

char	**add_string_to_2d_array(char **array, char *new_string)
{
	int		current_rows;
	char	**new_array;
	int		i;

	current_rows = 0;
	i = 0;
	if (array != NULL)
		current_rows = count_rows(array);
	new_array = malloc((current_rows + 2) * sizeof(char *));
	if (!new_array)
	{
		perror("malloc");
		return (NULL);
	}
	while (i < current_rows)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = ft_strdup(new_string);
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}

void	add_to_array_helper(t_parser *parser, int new_redirection)
{
	parser->redirection = (int *)malloc(sizeof(int) * 2);
	if (parser->redirection == NULL)
		return ;
	parser->redirection[0] = new_redirection;
	parser->redirection[1] = '\0';
}

void	add_to_array(t_parser *parser, int new_redirection)
{
	int	i;
	int	*new_array;
	int	j;

	i = 0;
	j = 0;
	if (parser->redirection == NULL)
		add_to_array_helper(parser, new_redirection);
	else
	{
		while (parser->redirection[i] != '\0')
			i++;
		new_array = (int *)malloc(sizeof(int) * (i + 2));
		if (new_array == NULL)
			return ;
		while (j < i)
		{
			new_array[j] = parser->redirection[j];
			j++;
		}
		new_array[i] = new_redirection;
		new_array[i + 1] = '\0';
		free(parser->redirection);
		parser->redirection = new_array;
	}
}
