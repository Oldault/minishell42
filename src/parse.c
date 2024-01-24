/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:38:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/24 15:08:07 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redirection_symbol(char *word)
{
    return (strcmp(word, "<") == 0 || strcmp(word, ">") == 0 || strcmp(word, ">>") == 0);
}

RedirectionType get_redirection_type(char *symbol)
{
    if (strcmp(symbol, "<") == 0) return REDIR_INPUT;
    if (strcmp(symbol, ">") == 0) return REDIR_OUTPUT;
    if (strcmp(symbol, ">>") == 0) return REDIR_APPEND;
    return REDIR_NONE; // Default case, should not occur
}

void free_redir_array(Redirection **redir_arr)
{
    if (redir_arr == NULL) return;
    int i = 0;
    while (redir_arr[i])
    {
        free(redir_arr[i]->filename);
        free(redir_arr[i]);
        i++;
    }
    free(redir_arr);
}

int starts_with_redirection_symbol(char *word)
{
    return (word[0] == '<' || word[0] == '>');
}

void split_redirection(char *word, Redirection **redirection)
{
    char *symbol = (char *)malloc(3 * sizeof(char)); // Maximum length of a redirection symbol is 2 characters
    int i = 0;

    // Extract the redirection symbol
    while (word[i] == '<' || word[i] == '>')
    {
        symbol[i] = word[i];
        i++;
    }
    symbol[i] = '\0';

    (*redirection)->type = get_redirection_type(symbol);
    (*redirection)->filename = strdup(&word[i]);

    free(symbol);
}


char **parse_segment(char *segment, Redirection **redirection)
{
    char **words = ft_split(segment, ' ');
    if (words == NULL)
        return NULL;

    // Initialize redirection
    *redirection = (Redirection *)malloc(sizeof(Redirection));
    (*redirection)->type = REDIR_NONE;
    (*redirection)->filename = NULL;

    // Calculate the length of the command array
    int i = 0, cmd_len = 0;
    while (words[i])
    {
        if (!is_redirection_symbol(words[i]) && !starts_with_redirection_symbol(words[i]))
            cmd_len++;
        i++;
    }

    // Allocate memory for the command array
    char **cmd = (char **)malloc((cmd_len + 1) * sizeof(char *));
    if (cmd == NULL)
    {
        free_double_array(words);
        free(*redirection);
        return NULL;
    }

    // Fill the command array, handling redirection symbols
    i = 0;
    int j = 0;
    while (words[i])
    {
        if (is_redirection_symbol(words[i]))
        {
            (*redirection)->type = get_redirection_type(words[i]);
            (*redirection)->filename = strdup(words[i + 1]);
            i++; // Skip the next word (filename)
        }
        else if (starts_with_redirection_symbol(words[i]))
        {
            split_redirection(words[i], redirection);
        }
        else
        {
            cmd[j] = strdup(words[i]);
            j++;
        }
        i++;
    }
    cmd[j] = NULL; // Null-terminate the command array

    free_double_array(words);
    return cmd;
}



int	parse(t_mini *info)
{
	int		i;
	int		j;
	char	**segments;
	char	***cmd_arr;
	Redirection **redir_arr;

	segments = ft_split(info->input, '|');
	if (segments == NULL)
		return (-1);
	i = 0;
	while (segments[i])
		i++;
	cmd_arr = (char ***)malloc((i + 1) * sizeof(char **));
	redir_arr = (Redirection **)malloc((i + 1) * sizeof(Redirection *));
	if (cmd_arr == NULL || redir_arr == NULL)
		return (free_double_array(segments), -1);
	j = -1;
	while (++j < i)
	{
		// redirect(segments[j], info);
		// printf(": in_fd = %d, out_fd = %d\n", info->in_fd, info->out_fd);
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
