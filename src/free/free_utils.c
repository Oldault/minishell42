/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:32:35 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/14 10:46:43 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_exit(char *str)
{
	perror(str);
	exit(127);
}

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
	int	i;

	if (redir != NULL)
	{
		i = -1;
		while (++i < redir->count)
		{
			free(redir->redirs[i].filename);
		}
		free(redir->redirs);
		redir->redirs = NULL;
		redir->count = 0;
	}
}
