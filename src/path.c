/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:08:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/06 13:06:15 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char **paths, char **arg)
{
	int		i;
	char	*path;
	char	*temp;

	path = NULL;
	if (arg[0][0] == '/' || arg[0][0] == '.')
	{
		return (ft_strdup(arg[0]));
	}
	if (!paths)
	{
		return (NULL);
	}
	for (i = 0; paths[i]; ++i)
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			return (NULL);
		}
		path = ft_strjoin(temp, arg[0]);
		free(temp); // Free temp after it's used
		if (!path)
		{
			return (NULL);
		}
		if (access(path, F_OK | X_OK) == 0)
		{
			return (path);
		}
		free(path);
	}
	return (NULL);
}

char	**get_paths(t_mini *data, char **env)
{
	int		i;
	char	*full_path;
	char	**paths;

	(void)data;
	i = -1;
	full_path = NULL;
	paths = NULL;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			full_path = ft_strdup(env[i] + 5);
			if (!full_path)
				return (NULL);
			break ;
		}
	}
	if (!full_path)
		return (NULL);
	paths = ft_split(full_path, ':');
	if (!paths)
		return (NULL);
	free(full_path);
	return (paths);
}
