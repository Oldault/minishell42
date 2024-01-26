/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:36:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/26 15:42:26 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_start(char *word)
{
	return (strchr(word, '<') || strchr(word, '>'));
}

int	redir_symb(char *word)
{
	return (ft_strcmp(word, "<") == 0 || ft_strcmp(word, ">") == 0
		|| ft_strcmp(word, ">>") == 0);
}

re_type	redir_type(char *symbol)
{
	if (ft_strcmp(symbol, "<") == 0)
		return (REDIR_INPUT);
	if (ft_strcmp(symbol, ">") == 0)
		return (REDIR_OUTPUT);
	if (ft_strcmp(symbol, ">>") == 0)
		return (REDIR_APPEND);
	return (REDIR_NONE);
}

int	get_cmd_len(char **words)
{
	int	cmd_len;
	int	i;

	i = -1;
	cmd_len = 0;
	while (words[++i])
	{
		if (!redir_symb(words[i]) && !redir_start(words[i]))
			cmd_len++;
	}
	return (cmd_len);
}

void	redir_split(char *word, redir_t *redirection)
{
	char	*symbol;
	int		i;

	i = 0;
	while (word[i] == '<' || word[i] == '>')
		i++;
	symbol = strndup(word, i);
	redirection->type = redir_type(symbol);
	redirection->filename = ft_strdup(&word[i]);
	free(symbol);
}
