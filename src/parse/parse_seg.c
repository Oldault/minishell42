/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_seg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:20:20 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 13:12:30 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_segment_parsing(char ***cmd, t_redirs *redirections,
		char **words, int cmd_len)
{
	int	max_redirs;

	max_redirs = 10;
	*cmd = (char **)ft_calloc((cmd_len + 1), sizeof(char *));
	if (*cmd == NULL)
		return (free_double_array(words), -1);
	redirections->redirs = (t_rdr *)ft_calloc(max_redirs, sizeof(t_rdr));
	if (redirections->redirs == NULL)
		return (free_double_array(words), -1);
	redirections->redirs->filename = NULL;
	redirections->redirs->type = REDIR_NONE;
	redirections->count = 0;
	return (0);
}

static void	process_redirection(t_rdr *redir, char **words, int *i)
{
	if (redir_start(words[*i]) && !redir_symb(words[*i]))
		redir_split(words[*i], redir);
	else
	{
		redir->type = redir_type(words[*i]);
		if (words[*i + 1] && !redir_symb(words[*i + 1]))
		{
			redir->filename = ft_strdup(words[*i + 1]);
			(*i)++;
		}
		else
			redir->filename = NULL;
	}
}

static void	process_segment_parts(char **words, char ***cmd,
		t_redirs *redirections)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	j = 0;
	k = 0;
	while (words[++i])
	{
		if (redir_symb(words[i]) || redir_start(words[i]))
		{
			process_redirection(&redirections->redirs[k], words, &i);
			k++;
		}
		else
			(*cmd)[j++] = ft_strdup(words[i]);
	}
	redirections->count = k;
	(*cmd)[j] = NULL;
}

int var_name_length(char *input)
{
    int len = 0;
    while (input[len] && input[len] != ' ' && input[len] != '$') 
	{
        len++;
    }
    return len;
}

char	**parse_segment(t_mini *data, char *segment, t_redirs *redirections)
{
	char	**words;
	char	**cmd;
	int		cmd_len;

	words = parse_command_segment(segment, data->env);
	if (!words)
		return (NULL);
	cmd_len = get_cmd_len(words);
	if (init_segment_parsing(&cmd, redirections, words, cmd_len) != 0)
		return (NULL);
	process_segment_parts(words, &cmd, redirections);
	free_double_array(words);
	return (cmd);
}
