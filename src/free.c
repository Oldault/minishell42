/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:27:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/06 12:39:44 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_array(char **array)
{
	int	i;

	i = -1;
	if (array == NULL)
		return ;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	free_triple_array(char ***array)
{
	int	i;

	i = -1;
	if (array != NULL)
	{
		while (array[++i])
			free_double_array(array[i]);
		free(array);
	}
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

void	free_mini(t_mini *data)
{
    if (data != NULL)
    {
        free(data->input);
        data->input = NULL;
        free(data->prompt);
        data->prompt = NULL;
        free_double_array(data->env);
        data->env = NULL;
        free_double_array(data->paths);
        data->paths = NULL;
        free_cmds(&data->cmds);
        data->cmds = NULL;
    }
    if (data->redir != NULL)
    {
        for (int i = 0; i < data->redir->count; i++)
        {
            free(data->redir->redirs[i].filename);
            data->redir->redirs[i].filename = NULL;
        }
        free(data->redir->redirs);
        data->redir->redirs = NULL;
        free(data->redir);
        data->redir = NULL;
    }
	if (data->bltn != NULL)
	{
		free(data->bltn);
		data->bltn = NULL;
	}
	if (data->err != NULL)
	{
		free(data->err);
		data->err = NULL;
	}
    free(data);
    data = NULL;
}

void	free_redir_array(t_redirs *redirections)
{
	int	i;

	i = -1;
	if (redirections == NULL || redirections->redirs == NULL)
		return ;
	while (++i < redirections->count)
	{
		free(redirections->redirs[i].filename);
		redirections->redirs[i].filename = NULL;
	}
	free(redirections->redirs);
}
