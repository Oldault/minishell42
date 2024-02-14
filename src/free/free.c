/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:27:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/14 10:47:02 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_data_in(t_mini *data)
{
	int	i;

	free(data->input);
	free(data->prompt);
	free_double_array(data->paths);
	free_cmds(&data->cmds);
	i = -1;
	while (++i < data->seg_count)
		reset_redirections(&data->redir[i]);
	free(data->redir);
	if (data->err)
		free(data->err);
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

	if (data->input)
		free(data->input);
	if (data->prompt)
		free(data->prompt);
	free_double_array(data->env);
	free_double_array(data->paths);
	if (data->cmds)
	{
		i = -1;
		while (++i < data->seg_count)
		{
			free_double_array(data->cmds[i]);
		}
		free(data->cmds);
	}
	free_builtins(&data->bltn);
	free(data);
}
