/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:27:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/06 14:46:32 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_array(char **array)
{
	int	i;

	i = -1;
	if (array == NULL || *array == NULL)
		return ;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	free_triple_array(char ***array)
{
	int	i;

	i = -1;
	if (array == NULL || *array == NULL || **array == NULL)
		return ;
	while (array[++i])
		free_double_array(array[i]);
	free(array);
}

void	free_cmds(char ****cmds)
{
	int	i;
	int	j;

	i = -1;
	if (cmds == NULL || *cmds == NULL)
		return ;
	while ((*cmds)[++i] != NULL)
	{
		j = -1;
		while ((*cmds)[i][++j] != NULL)
		{
			free((*cmds)[i][j]);
			(*cmds)[i][j] = NULL;
		}
		free((*cmds)[i]);
		(*cmds)[i] = NULL;
	}
	free(*cmds);
	*cmds = NULL;
}

void	reset_redirections(t_redirs *redir)
{
	if (redir != NULL)
	{
		for (int i = 0; i < redir->count; i++)
		{
			free(redir->redirs[i].filename);
		}
		free(redir->redirs);
		redir->redirs = NULL;
		redir->count = 0;
	}
}

void	reset_data_in(t_mini *data)
{
	free(data->input);
	free(data->prompt);
	free_double_array(data->paths);
	free_cmds(&data->cmds);
	reset_redirections(data->redir);
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
	if (data->input)
		free(data->input);
	if (data->prompt)
		free(data->prompt);
	free_double_array(data->env);
	free_double_array(data->paths);
	if (data->cmds)
	{
		for (int i = 0; i < data->seg_count; ++i)
		{
			free_double_array(data->cmds[i]);
		}
		free(data->cmds);
	}
	if (data->redir)
	{
		for (int i = 0; i < data->seg_count; ++i)
		{
			reset_redirections(&data->redir[i]);
		}
		free(data->redir);
	}
	free_builtins(&data->bltn);
	free(data);
}
