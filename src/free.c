/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:27:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/22 20:35:18 by svolodin         ###   ########.fr       */
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

void	free_mini(t_mini *info)
{
	free(info->prompt);
	free(info->paths);
}
