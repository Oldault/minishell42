/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:27:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/14 15:36:07 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_data_mid(t_mini *data)
{
	free(data->input);
	data->input = NULL;
	free_double_array(data->paths);
	free(data->prompt);
	data->prompt = NULL;
}

void	reset_data_in(t_mini *data)
{
	int	i;

	i = 0;
	reset_data_mid(data);
	free_cmds(&data->cmds);
	while (i < data->seg_count)
	{
		reset_redirections(&data->redir[i]);
		i++;
	}
	free(data->redir);
	data->redir = NULL;
	if (data->err)
	{
		free(data->err);
		data->err = NULL;
	}
}

void	free_builtins(t_cmd_entry **builtins)
{
	if (*builtins != NULL)
	{
		free(*builtins);
		*builtins = NULL;
	}
}

void	free_seg_cmd_redir(char **seg, char ***cmd, t_redirs *redir)
{
	free_double_array(seg);
	free_cmds(&cmd);
	reset_redirections(redir);
}

void	reset_data_out(t_mini *data)
{
	int	i;

	i = -1;
	if (!data)
		return ;
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->prompt)
	{
		free(data->prompt);
		data->prompt = NULL;
	}
	free_double_array(data->env);
	free_double_array(data->paths);
	if (data->cmds)
	{
		while (++i < data->seg_count)
			free_double_array(data->cmds[i]);
		free(data->cmds);
		data->cmds = NULL;
	}
	free_builtins(&data->bltn);
	free(data);
}
