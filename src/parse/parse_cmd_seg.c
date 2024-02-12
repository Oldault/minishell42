/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_seg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:19:48 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 17:21:48 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_seg	*init_pdata(char *segment)
{
	t_parse_seg	*pdata;

	pdata = malloc(sizeof(t_parse_seg));
	pdata->args = malloc(sizeof(char *) * (ft_strlen(segment) / 2 + 2));
	for (int i = 0; i < ((int)ft_strlen(segment) / 2 + 2); ++i)
	{
		pdata->args[i] = NULL; // Initialize each element to NULL
	}
	pdata->current_arg = malloc(strlen(segment) + 1);
	pdata->current_arg[0] = '\0';
	pdata->c = '\0';
	pdata->in_single_quote = 0;
	pdata->in_double_quote = 0;
	pdata->arg_count = 0;
	pdata->current_length = 0;
	pdata->should_expand = 1;
	return (pdata);
}

char	*expand_variable(char *var, char **env, int expand)
{
	if (!expand)
		return (strdup(var));
	for (int i = 0; env && env[i]; i++)
	{
		if (strncmp(env[i], var, strlen(var)) == 0
			&& env[i][strlen(var)] == '=')
		{
			return (strdup(env[i] + strlen(var) + 1));
		}
	}
	return (NULL);
}

int	handle_single_quote(t_parse_seg *pdata)
{
	if (pdata->c == '\'' && !pdata->in_double_quote)
	{
		pdata->in_single_quote = !pdata->in_single_quote;
		pdata->should_expand = !pdata->in_single_quote;
		return (1);
	}
	return (0);
}

int	handle_double_quote(t_parse_seg *pdata)
{
	if (pdata->c == '\"' && !pdata->in_single_quote)
	{
		pdata->in_double_quote = !pdata->in_double_quote;
		return (1);
	}
	return (0);
}

int	handle_space(t_parse_seg *pdata)
{
	if (pdata->c == ' ' && !pdata->in_single_quote && !pdata->in_double_quote)
	{
		if (pdata->current_length > 0)
		{
			pdata->current_arg[pdata->current_length] = '\0';
			pdata->args[pdata->arg_count++] = ft_strdup(pdata->current_arg);
			pdata->current_length = 0;
		}
		return (1);
	}
	return (0);
}

void	handle_expanded_value(t_parse_seg *pdata, char *expanded_value)
{
	if (pdata->current_length > 0)
	{
		pdata->current_arg[pdata->current_length] = '\0';
		pdata->args[pdata->arg_count++] = strdup(pdata->current_arg);
		pdata->current_length = 0;
	}
	pdata->args[pdata->arg_count++] = expanded_value;
}

void	handle_nonexistent_variable(t_parse_seg *pdata, char *var_name,
		size_t var_len)
{
	pdata->current_arg[pdata->current_length++] = '$';
	strncpy(pdata->current_arg + pdata->current_length, var_name, var_len);
	pdata->current_length += var_len;
	pdata->current_arg[pdata->current_length] = '\0';
}

int	handle_expansion(char *segment, size_t *i, t_parse_seg *pdata, char **env)
{
	char	var_name[1024];
	char	*expanded_value;
	size_t	var_len;
	size_t	segment_len;

	var_len = 0;
	segment_len = strlen(segment);
	if (pdata->c == '$' && pdata->should_expand)
	{
		(*i)++;
		while (*i < segment_len && var_len < 1023 && (ft_isalnum(segment[*i])
				|| segment[*i] == '_' || segment[*i] == '?'))
		{
			var_name[var_len++] = segment[(*i)++];
		}
		var_name[var_len] = '\0';
		if (*i >= segment_len)
		{
			(*i) = segment_len;
		}
		expanded_value = expand_variable(var_name, env, pdata->should_expand);
		if (expanded_value)
		{
			handle_expanded_value(pdata, expanded_value);
		}
		else
		{
			handle_nonexistent_variable(pdata, var_name, var_len);
		}
		if (*i > 0 && *i < segment_len && segment[*i] != '\0'
			&& segment[*i] != ' ')
		{
			(*i)--;
		}
		return (1);
	}
	return (0);
}

char	**parse_command_segment(char *segment, char **env)
{
	t_parse_seg *pdata = init_pdata(segment);
	if (!pdata)
		return (NULL);

	size_t segment_len = strlen(segment);
	for (size_t i = 0; i < segment_len; ++i)
	{
		pdata->c = segment[i];

		if (handle_single_quote(pdata) || handle_double_quote(pdata)
			|| handle_space(pdata) || handle_expansion(segment, &i, pdata, env))
		{
			continue ;
		}

		pdata->current_arg[pdata->current_length++] = pdata->c;
	}

	if (pdata->current_length > 0)
	{
		pdata->current_arg[pdata->current_length] = '\0';
		pdata->args[pdata->arg_count++] = strdup(pdata->current_arg);
	}

	free(pdata->current_arg);
	char **temp_args = pdata->args;
	free(pdata);
	return (temp_args);
}