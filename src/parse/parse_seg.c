/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_seg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:20:20 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 11:18:33 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_segment_parsing(char ***cmd, redirs_t *redirections,
		char **words, int cmd_len)
{
	int	max_redirs;

	max_redirs = 10;
	*cmd = (char **)malloc((cmd_len + 1) * sizeof(char *));
	redirections->redirs = (redir_t *)malloc(max_redirs * sizeof(redir_t));
	redirections->count = 0;
	if (*cmd == NULL || redirections->redirs == NULL)
	{
		if (*cmd != NULL)
			free(*cmd);
		if (redirections->redirs != NULL)
			free(redirections->redirs);
		return (free_double_array(words), -1);
	}
	return (0);
}

static void	process_redirection(redir_t *redir, char **words, int *i)
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
		redirs_t *redirections)
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
			(*cmd)[j++] = strdup(words[i]);
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

char	**parse_segment(t_mini *data, char *segment, redirs_t *redirections)
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
