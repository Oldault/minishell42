/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_trim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albeninc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 13:25:19 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/20 13:34:43 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vérifie si la position actuelle correspond à un token spécial
int	is_special_token(const char *str)
{
	const char *special_tokens[] = {">>", "<<", "||", "&&", "*", ">", "<", "|", NULL};
	
	for (int i = 0; special_tokens[i] != NULL; i++)
	{
		if (strncmp(str, special_tokens[i], strlen(special_tokens[i])) == 0) {
            		return strlen(special_tokens[i]);
	}
    }
	return (0);
}

// Tokenizer principal
char	**tokenizer(const char *str)
{
	int	bufsize = 64;
	int	start;
	int	position = 0;
	char	**tokens = malloc(bufsize * sizeof(char*));
	char	*token;
	int	length;
	int	i = 0;

	if (!tokens) 
	{
        	fprintf(stderr, "Allocation error\n");
        	exit(EXIT_FAILURE);
	}

	while (str[i] != '\0')
	{
		while (str[i] == ' ' || str[i] == '\t') i++;  // Ignorer les espaces
        	length = is_special_token(&str[i]);
		if (length > 0) 
		{  // Token spécial trouvé
			token = strndup(&str[i], length);
			i += length;  // Passer le token spécial
		}
		else
		{  // Autres caractères
            		start = i;
            		while (str[i] != ' ' && str[i] != '\0' && !is_special_token(&str[i])) i++;
            			token = strndup(&str[start], i - start);
		}
        	tokens[position++] = token;
		if (position >= bufsize)
		{
            		bufsize += 64;
            		tokens = realloc(tokens, bufsize * sizeof(char*));
            		if (!tokens)
			{
                		fprintf(stderr, "Allocation error\n");
                		exit(EXIT_FAILURE);
            		}
        	}
    	}
	tokens[position] = NULL;
	return (tokens);
}

int main() {
    char *input = "cat file1.txt || echo | echo > echo * >> output.txt";
    char **tokens = tokenizer(input);

    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: %s\n", i, tokens[i]);
        free(tokens[i]);
    }
    free(tokens);
    return 0;
}
