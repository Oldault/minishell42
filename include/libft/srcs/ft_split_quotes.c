/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:53 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 13:06:39 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	unleah(char **str, int size)
{
	while (size >= 0)
		free(str[size--]);
	free(str);
	return (-1);
}

static int	count_words(const char *str, char charset)
{
	int	words;
	int	in_quote;

	words = 0;
	in_quote = 0;
	for (int i = 0; str[i];)
	{
		while (str[i] == charset && !in_quote)
			i++;
		if (str[i] == '\0')
			break ;
		if (str[i] == '\"')
			in_quote = !in_quote;
		words++;
		while (str[i] && (in_quote || str[i] != charset))
		{
			if (str[i] == '\"')
				in_quote = !in_quote;
			i++;
		}
	}
	return (words);
}

static void	write_word(char *dest, const char *from, char charset)
{
	int	i;
	int	j;
	int	in_quote;

	i = 0;
	j = 0;
	in_quote = 0;
	while (from[i] && (in_quote || from[i] != charset))
	{
		if (from[i] == '\"')
		{
			in_quote = !in_quote; // Toggle quote flag
			i++;
			continue ;
		}
		dest[j++] = from[i++];
	}
	dest[j] = '\0';
}

static int	write_split(char **split, const char *str, char charset)
{
	int	i;
	int	in_quote;
	int	start;
	int	word_length;
	int	word_index;

	i = 0;
	word_index = 0;
	in_quote = 0;
	while (str[i])
	{
		while (str[i] == charset && !in_quote)
			i++;
		if (str[i] == '\0')
			break ;
		start = i;
		while (str[i] && (in_quote || str[i] != charset))
		{
			if (str[i] == '\"')
				in_quote = !in_quote;
			i++;
		}
		word_length = i - start;
		split[word_index] = (char *)malloc(sizeof(char) * (word_length + 1));
		if (!split[word_index])
			return (unleah(split, word_index - 1));
		write_word(split[word_index], str + start, charset);
		word_index++;
	}
	split[word_index] = NULL;
	return (0);
}

char	**ft_split_quotes(const char *str, char c)
{
	char	**res;
	int		words;

	words = count_words(str, c);
	res = ft_calloc(words + 1, sizeof(char *));
	if (!res)
		return (NULL);
	res[words] = 0;
	if (write_split(res, str, c) == -1)
	{
		free(res);
		return (NULL);
	}
	return (res);
}
