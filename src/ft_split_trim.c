/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_trim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 13:25:19 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/20 15:25:15 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

// Vérifie si la position actuelle correspond à un token spécial
static int	is_special_token(const char *str)
{
	const char	*special_tokens[] = {">>", "<<", "||", "&&", "*", ">", "<", "|", NULL};
	int			i;

	i = 0;
	while (special_tokens[i])
	{
		if (ft_strncmp(str, special_tokens[i], ft_strlen(special_tokens[i])) == 0)
			return ft_strlen(special_tokens[i]);
		i++;
	}
	return (0);
}

// Tokenizer principal
char	**tokenizer(const char *str)
{
	int		bufsize;
	int		start;
	int		position;
	char	**tokens;
	char	*token;
	int		length;
	int		i;

	i = 0;
	bufsize = 64;
	tokens = malloc(bufsize * sizeof(char *));
	position = 0;
	if (!tokens) 
	{
		ft_putendl_fd("Allocation error", 2);
		exit(EXIT_FAILURE);
	}
	while (str[i] != '\0')
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++; // Ignorer les espaces
		length = is_special_token(&str[i]);
		if (length > 0)
		{  // Token spécial trouvé
			token = strndup(&str[i], length);
			i += length;  // Passer le token spécial
		}
		else
		{ // Autres caractères
            start = i;
			while (str[i] != ' ' && str[i] != '\0' && !is_special_token(&str[i]))
				i++;
			token = strndup(&str[start], i - start);
		}
		tokens[position++] = token;
		if (position >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens)
			{
				printf("Allocation error\n");
				exit(EXIT_FAILURE);
        	}
    	}
	}
	tokens[position] = NULL;
	return (tokens);
}

// int main(void)
// {
//     char *input = "cat file1.txt ||echo|echo>>>>>echo* >> |||**output.txt";
//     char **tokens = tokenizer(input);

//     for (int i = 0; tokens[i] != NULL; i++)
// 	{
//         printf("Token %d: %s\n", i, tokens[i]);
//         free(tokens[i]);
//     }
//     free(tokens);
//     return 0;
// }
