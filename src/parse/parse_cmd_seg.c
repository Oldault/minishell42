/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_seg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:19:48 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 17:28:47 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_seg	*init_pdata(char *segment)
{
	t_parse_seg	*pdata;

	pdata = ft_calloc(1, sizeof(t_parse_seg));
	if (!pdata)
		return (NULL);
	pdata->args = ft_calloc((ft_strlen(segment) + 1), sizeof(char *));
	if (!pdata->args)
	{
		free(pdata);
		return (NULL);
	}
	pdata->current_arg = ft_calloc(ft_strlen(segment) + 1, sizeof(char));
	if (!pdata->current_arg)
	{
		free(pdata->args);
		free(pdata);
		return (NULL);
	}
	pdata->should_expand = 1;
	return (pdata);
}

char	*expand_variable(char *var, char **env, int expand)
{
	int	i;

	if (!expand)
		return (ft_strdup(var));
	if (ft_strcmp(var, "?") == 0)
	{
		return (ft_itoa(g_exit_stat));
	}
	i = -1;
	while (env && env[++i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0
			&& env[i][ft_strlen(var)] == '=')
			return (ft_strdup(env[i] + ft_strlen(var) + 1));
	}
	return (NULL);
}

static void	parse_cmd_segment(t_parse_seg *pdata, char *segment, char **env)
{
	size_t	segment_len;
	size_t	i;

	segment_len = ft_strlen(segment);
	i = -1;
	while (++i < segment_len)
	{
		pdata->c = segment[i];
		if (handle_single_quote(pdata) || handle_double_quote(pdata)
			|| handle_space(pdata) || handle_expansion(segment, &i, pdata, env))
			continue ;
		if (pdata->current_length < (int)segment_len)
			pdata->current_arg[pdata->current_length++] = pdata->c;
	}
}

char	**parse_command_segment(char *segment, char **env)
{
	char		**temp_args;
	t_parse_seg	*pdata;

	pdata = init_pdata(segment);
	if (!pdata)
		return (NULL);
	parse_cmd_segment(pdata, segment, env);
	if (pdata->current_length > 0)
	{
		pdata->current_arg[pdata->current_length] = '\0';
		pdata->args[pdata->arg_count++] = ft_strdup(pdata->current_arg);
	}
	pdata->args[pdata->arg_count] = NULL;
	free(pdata->current_arg);
	temp_args = pdata->args;
	free(pdata);
	return (temp_args);
}
