/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:38:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/24 16:23:46 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_redir_array(redir_t **redir_arr)
{
	int	i;

	if (redir_arr == NULL)
		return ;
	i = -1;
	while (redir_arr[++i])
	{
		free(redir_arr[i]->filename);
		free(redir_arr[i]);
	}
	free(redir_arr);
}

int	redir_start(char *word)
{
	return (word[0] == '<' || word[0] == '>');
}

void	redir_split(char *word, redir_t **redirection)
{
	int		i;
	char	*symbol;

	symbol = (char *)malloc(3 * sizeof(char));
	i = 0;
	while (word[i] == '<' || word[i] == '>')
	{
		symbol[i] = word[i];
		i++;
	}
	symbol[i] = '\0';
	(*redirection)->type = redir_type(symbol);
	(*redirection)->filename = strdup(&word[i]);
	free(symbol);
}

int     get_cmd_len(char **words)
{
    int cmd_len;
    int i;

    i = -1;
    cmd_len = 0;
    while (words[++i])
    {
        if (!redir_symb(words[i]) && !redir_start(words[i]))
			cmd_len++;
    }
    return (cmd_len);
}

char	**parse_segment(char *segment, redir_t **redirection)
{
	char	**words;
	char	**cmd;
	int		i;
	int		j;

	words = ft_split(segment, ' ');
	if (words == NULL)
		return (NULL);
    
	*redirection = (redir_t *)malloc(sizeof(redir_t));
	(*redirection)->type = REDIR_NONE;
	(*redirection)->filename = NULL;

	cmd = (char **)malloc((get_cmd_len(words) + 1) * sizeof(char *));
	if (cmd == NULL)
		return (free_double_array(words), free(*redirection), NULL);
	i = -1;
	j = 0;
	while (words[++i])
	{
		if (redir_symb(words[i]))
		{
			(*redirection)->type = redir_type(words[i]);
			(*redirection)->filename = strdup(words[i + 1]);
			i++;
		}
		else if (redir_start(words[i]))
			redir_split(words[i], redirection);
		else
			cmd[j++] = strdup(words[i]);
	}
	cmd[j] = NULL;
	free_double_array(words);
	return (cmd);
}

int	parse(t_mini *info)
{
	int		i;
	int		j;
	char	**segments;
	char	***cmd_arr;
	redir_t	**redir_arr;

	segments = ft_split(info->input, '|');
	if (segments == NULL)
		return (-1);
	i = 0;
	while (segments[i])
		i++;
	cmd_arr = (char ***)malloc((i + 1) * sizeof(char **));
	redir_arr = (redir_t **)malloc((i + 1) * sizeof(redir_t *));
	if (cmd_arr == NULL || redir_arr == NULL)
		return (free_double_array(segments), -1);
	j = -1;
	while (++j < i)
	{
		cmd_arr[j] = parse_segment(segments[j], &redir_arr[j]);
		if (cmd_arr[j] == NULL)
			return (free(segments[j]), free_triple_array(cmd_arr), -1);
	}
	free_double_array(segments);
	cmd_arr[j] = NULL;
	redir_arr[j] = NULL;
	info->cmds = cmd_arr;
	info->redir = redir_arr;
	return (0);
}
