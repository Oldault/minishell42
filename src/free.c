/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:27:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/27 11:35:09 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_array(char **array)
{
	int	i;

	i = -1;
	if (array != NULL)
	{
		while (array[++i])
			free(array[i]);
		free(array);
	}
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
	j = -1;
	while ((*cmds)[++i] != NULL)
	{
		while ((*cmds)[i][++j] != NULL)
			free((*cmds)[i][j]);
		free((*cmds)[i]);
	}
	free(*cmds);
}

void	free_mini(t_mini *data)
{
	free(data->prompt);
	free(data->paths);
}

void	free_redir_array(redirs_t *redirections)
{
	int	i;

	i = -1;
	if (redirections == NULL || redirections->redirs == NULL)
		return ;
	while (++i < redirections->count)
		free(redirections->redirs[i].filename);
	free(redirections->redirs);
}
