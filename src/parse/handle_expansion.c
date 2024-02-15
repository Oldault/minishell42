/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:44:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 10:34:17 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	extract_variable_name(char *segment, size_t *i, char *var_name,
		size_t segment_len)
{
	size_t	var_len;

	var_len = 0;
	while (*i < segment_len && var_len < 1023 && (ft_isalnum(segment[*i])
			|| segment[*i] == '_' || segment[*i] == '?'))
	{
		var_name[var_len++] = segment[(*i)++];
	}
	var_name[var_len] = '\0';
}

static void	split_and_store_arguments(t_parse_seg *pdata)
{
	char	**splitted;
	int		i;

	splitted = ft_split(pdata->current_arg, ' ');
	i = -1;
	while (splitted && splitted[++i])
	{
		pdata->args[pdata->arg_count++] = ft_strdup(splitted[i]);
		free(splitted[i]);
	}
	free(splitted);
	pdata->current_arg[0] = '\0';
	pdata->current_length = 0;
}

static void	handle_expanded_value(t_parse_seg *pdata, char *expanded_value)
{
	char	*temp;

	temp = ft_strjoin(pdata->current_arg, expanded_value);
	if (temp)
	{
		free(pdata->current_arg);
		pdata->current_arg = temp;
	}
	if (pdata->current_length > 0)
		split_and_store_arguments(pdata);
	else
		pdata->args[pdata->arg_count++] = ft_strdup(expanded_value);
	free(expanded_value);
}

static void	adjust_index(char *segment, size_t *i, size_t segment_len)
{
	if (*i > 0 && *i < segment_len && segment[*i] != '\0' && segment[*i] != ' ')
	{
		(*i)--;
	}
}

int	handle_expansion(char *segment, size_t *i, t_parse_seg *pdata, char **env)
{
	char	var_name[1024];
	char	*expanded_value;
	size_t	var_len;
	size_t	segment_len;

	var_len = 0;
	segment_len = ft_strlen(segment);
	if (pdata->c == '$' && pdata->should_expand)
	{
		(*i)++;
		extract_variable_name(segment, i, var_name, segment_len);
		if (*i >= segment_len)
			(*i) = segment_len;
		expanded_value = expand_variable(var_name, env, pdata->should_expand);
		if (expanded_value)
			handle_expanded_value(pdata, expanded_value);
		else
			handle_nonexistent_variable(pdata, var_name, var_len);
		adjust_index(segment, i, segment_len);
		return (1);
	}
	return (0);
}
