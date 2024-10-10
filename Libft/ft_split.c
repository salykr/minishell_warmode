/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:57:19 by skreik            #+#    #+#             */
/*   Updated: 2024/06/13 17:57:20 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	count;
	int	flag;
	int	i;

	i = 0;
	count = 0;
	flag = 0;
	while (s[i])
	{
		if (s[i] != c && !flag)
		{
			flag = 1;
			count++;
		}
		else if (s[i] == c)
			flag = 0;
		i++;
	}
	return (count);
}

static void	ft_free_split(char **split, int word_index)
{
	int	i;

	i = 0;
	while (i < word_index)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	*ft_extract_word(const char **s, char c)
{
	char		*word;
	const char	*word_start;
	size_t		word_len;

	while (**s == c && **s != '\0')
		(*s)++;
	word_start = *s;
	word_len = 0;
	while (**s != c && **s != '\0')
	{
		(*s)++;
		word_len++;
	}
	word = (char *)malloc(word_len + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, word_start, word_len + 1);
	return (word);
}

static char	**ft_create_split(const char *s, char c, int words)
{
	char	**split;
	char	*word;
	int		word_index;

	split = (char **)malloc((words + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	word_index = 0;
	while (*s)
	{
		if (*s != c)
		{
			word = ft_extract_word(&s, c);
			if (!word)
			{
				ft_free_split(split, word_index);
				return (NULL);
			}
			split[word_index++] = word;
		}
		else
			s++;
	}
	split[word_index] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	int	words;

	if (!s)
		return (NULL);
	words = ft_count_words(s, c);
	return (ft_create_split(s, c, words));
}
