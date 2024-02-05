/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_seg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:19:48 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 11:56:52 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_seg	*init_pdata(char *segment)
{
	t_parse_seg	*pdata;

	pdata = malloc(sizeof(t_parse_seg));
	pdata->args = malloc(sizeof(char *) * (strlen(segment) / 2 + 2));
	pdata->current_arg = malloc(strlen(segment) + 1);
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

int	handle_expansion(char *segment, int *i, t_parse_seg *pdata, char **env)
{
	char	var_name[256];
	int		var_len;
	char	*expanded_value;

	var_len = 0;
	if (pdata->c == '$' && pdata->should_expand)
	{
		(*i)++;
		while (segment[*i] && (ft_isalnum(segment[*i]) || segment[*i] == '_'))
		{
			var_name[var_len++] = segment[(*i)++];
		}
		var_name[var_len] = '\0';
		expanded_value = expand_variable(var_name, env, pdata->should_expand);
		strcpy(pdata->current_arg + pdata->current_length, expanded_value);
		pdata->current_length += strlen(expanded_value);
		free(expanded_value);
		(*i)--;
		return (1);
	}
	return (0);
}

char	**parse_command_segment(char *segment, char **env)
{
	t_parse_seg	*pdata;

	pdata = init_pdata(segment);
	for (int i = 0; segment[i]; ++i)
	{
		pdata->c = segment[i];
		if (handle_single_quote(pdata))
			continue ;
		else if (handle_double_quote(pdata))
			continue ;
		else if (handle_space(pdata))
			continue ;
		else if (handle_expansion(segment, &i, pdata, env))
			continue ;
		pdata->current_arg[pdata->current_length++] = pdata->c;
	}
	if (pdata->current_length > 0)
	{
		pdata->current_arg[pdata->current_length] = '\0';
		pdata->args[pdata->arg_count++] = ft_strdup(pdata->current_arg);
	}
	free(pdata->current_arg);
	pdata->args[pdata->arg_count] = NULL;
	return (pdata->args);
}
