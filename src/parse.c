/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:38:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/26 13:11:18 by svolodin         ###   ########.fr       */
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

// Updated to handle an array of redir_t
void free_redir_array(redirs_t *redirections)
{
    if (redirections == NULL || redirections->redirs == NULL)
        return;

    for (int i = 0; i < redirections->count; ++i)
    {
        free(redirections->redirs[i].filename);
    }
    free(redirections->redirs);
}


int redir_start(char *word)
{
    return (strchr(word, '<') || strchr(word, '>'));
}

void redir_split(char *word, redir_t *redirection)
{
    int i = 0;
    while (word[i] == '<' || word[i] == '>')
        i++;
    
    char *symbol = strndup(word, i); // Extract the redirection symbol
    redirection->type = redir_type(symbol);
    redirection->filename = strdup(&word[i]); // Extract the filename
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

char **parse_segment(char *segment, redirs_t *redirections)
{
    char **words = ft_split(segment, ' ');
    if (words == NULL)
        return (NULL);

    int max_redirs = 10; // Arbitrary limit, adjust as needed
    redirections->redirs = (redir_t *)malloc(max_redirs * sizeof(redir_t));
    redirections->count = 0;

    int cmd_len = get_cmd_len(words);
    char **cmd = (char **)malloc((cmd_len + 1) * sizeof(char *));
    if (cmd == NULL)
    {
        free_double_array(words);
        free(redirections->redirs);
        return (NULL);
    }

    int i = 0, j = 0, k = 0;
    while (words[i])
    {
        if (redir_symb(words[i]) || redir_start(words[i]))
        {
            redir_t *redir = &redirections->redirs[k];
            if (redir_start(words[i]) && !redir_symb(words[i]))
                redir_split(words[i], redir);
            else
            {
                redir->type = redir_type(words[i]);
                if (words[i + 1] && !redir_symb(words[i + 1]))
                {
                    redir->filename = strdup(words[i + 1]);
                    printf("Detected separated redirection: %s, filename: %s\n", words[i], words[i + 1]);
                    i++; // Skip the filename
                }
                else
                    redir->filename = NULL;
            }
            k++;
        }
        else
            cmd[j++] = strdup(words[i]);
        i++;
    }
    redirections->count = k;
    cmd[j] = NULL;
    free_double_array(words);
    return (cmd);
}

int parse(t_mini *info)
{
    char **segments = ft_split(info->input, '|');
    if (segments == NULL)
        return (-1);

    // Count the number of segments
    int num_segments = 0;
    while (segments[num_segments])
        num_segments++;

    // Allocate arrays for commands and redirections
    char ***cmd_arr = (char ***)malloc((num_segments + 1) * sizeof(char **));
    redirs_t *redir_arr = (redirs_t *)malloc((num_segments + 1) * sizeof(redirs_t));
    if (cmd_arr == NULL || redir_arr == NULL)
    {
        free_double_array(segments);
        return (cmd_arr ? free(cmd_arr), -1 : free(redir_arr), -1);
    }

    // Parse each segment
    for (int i = 0; i < num_segments; ++i)
    {
        cmd_arr[i] = parse_segment(segments[i], &redir_arr[i]);
        if (cmd_arr[i] == NULL)
        {
            free_double_array(segments);
            for (int j = 0; j < i; ++j)
                free_double_array(cmd_arr[j]);
            free(cmd_arr);
            for (int j = 0; j <= i; ++j) // Free redirections up to the current index
                free_redir_array(&redir_arr[j]);
            free(redir_arr);
            return (-1);
        }
		printf("Segment %d: Command: ", i);
        for (int k = 0; cmd_arr[i][k] != NULL; k++)
            printf("%s ", cmd_arr[i][k]);
        printf("\nRedirections:\n");
        for (int k = 0; k < redir_arr[i].count; k++)
        {
            printf("Type: %d, Filename: %s\n", redir_arr[i].redirs[k].type, redir_arr[i].redirs[k].filename);
        }
    }

    // Set the end markers for the arrays
    cmd_arr[num_segments] = NULL;
    redir_arr[num_segments].redirs = NULL;
    redir_arr[num_segments].count = 0;

    // Assign the arrays to the info struct
    info->cmds = cmd_arr;
    info->redir = redir_arr;

    // Clean up
    free_double_array(segments);
    return (0);
}
