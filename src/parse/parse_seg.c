/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_seg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:20:20 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/02 18:42:34 by svolodin         ###   ########.fr       */
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

char *ensure_capacity(char *result, int current_len, int needed_len, int *chunk_size)
{
    if (current_len + needed_len >= *chunk_size) {
        while (current_len + needed_len >= *chunk_size) {
            *chunk_size *= 2;
        }
        char *new_result = (char *)calloc(1, *chunk_size);
        if (!new_result) return NULL;
        strcpy(new_result, result);
        free(result);
        result = new_result;
    }
    return result;
}

char *expand_doll(char *str, char **env) {
    int chunk_size = 1024;
    char *result = (char *)calloc(1, chunk_size);
    if (!result) return NULL;

    int result_len = 0;
    while (*str) {
        if (*str == '$') {
            str++;
            int name_len = var_name_length(str);
            char *name = strndup(str, name_len);
            char *value = get_env_value(name, env);
            if (value) {
                int value_len = strlen(value);
                result = ensure_capacity(result, result_len, value_len, &chunk_size);
                if (!result) return NULL;
                strcpy(result + result_len, value);
                result_len += value_len;
            }
            str += name_len;
            free(name);
        } else {
            result = ensure_capacity(result, result_len, 1, &chunk_size);
            if (!result) return NULL;
            result[result_len++] = *str++;
        }
    }
    result[result_len] = '\0'; // Null-terminate the result string.

    char *final_result = (char *)malloc(result_len + 1);
    if (final_result) {
        strcpy(final_result, result);
    }
    free(result);

    return final_result;
}

char	**parse_segment(t_mini *data, char *segment, redirs_t *redirections)
{
	char	**words;
	char	**cmd;
	int		cmd_len;

	segment = expand_doll(segment, data->env);
	words = ft_split(segment, ' ');
	if (!words)
		return (NULL);
	cmd_len = get_cmd_len(words);
	if (init_segment_parsing(&cmd, redirections, words, cmd_len) != 0)
		return (NULL);
	process_segment_parts(words, &cmd, redirections);
	free_double_array(words);
	return (cmd);
}
