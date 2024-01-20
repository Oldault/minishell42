/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_comandtrim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:59:00 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/19 18:04:11 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_COMMAND_LENGTH 1024
#define DELIMITERS " \t\r\n\a"

char	**custom_tokenize(char *input)
{
	int		bufsize;
	int		position;
	char	**tokens;
	char	*token;
	int		start;
	int		end;
	char	**new_tokens;

	bufsize = 64;
	position = 0;
	tokens = ft_calloc(bufsize, sizeof(char *));
	start = 0;
	end = 0;
	new_tokens = ft_calloc(bufsize, sizeof(char *));
	if (!tokens)
	{
		perror("custom_tokenize: allocation error");
		exit(EXIT_FAILURE);
	}
	while (input[end] != '\0')
	{
		if (ft_strchr(DELIMITERS, input[end]) != NULL)
		{
			if (end > start)
			{
				token = malloc((end - start + 1) * sizeof(char));
				if (!token)
				{
					perror("custom_tokenize: allocation error");
					exit(EXIT_FAILURE);
				}
				ft_strncpy(token, &input[start], end - start);
				token[end - start] = '\0';
				tokens[position++] = token;
				if (position >= bufsize)
				{
					bufsize += 64;
					if (!new_tokens)
					{
						perror("custom_tokenize: allocation error");
						exit(EXIT_FAILURE);
					}
					ft_memcpy(new_tokens, tokens, position * sizeof(char *));
					free(tokens);
					tokens = new_tokens;
				}
			}
			start = end + 1;
		}
		end++;
	}
	if (end > start)
	{
		token = malloc((end - start + 1) * sizeof(char));
		if (!token)
		{
			perror("custom_tokenize: allocation error");
			exit(EXIT_FAILURE);
		}
		ft_strncpy(token, &input[start], end - start);
		token[end - start] = '\0';
		tokens[position++] = token;
	}
	tokens[position] = NULL;
	return (tokens);
}

