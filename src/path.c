/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:08:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 07:25:27 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char **paths, char **arg)
{
	int		i;
	char	*path;
	char	*temp;

	i = -1;
	path = NULL;
	temp = NULL;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		path = ft_strjoin(temp, arg[0]);
		free(temp);
		if (!path)
			return (NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
	}
	return (NULL);
}

char	**get_paths(char **env)
{
	int		i;
	char	*full_path;
	char	**paths;

	i = -1;
	full_path = NULL;
	paths = NULL;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			full_path = ft_strdup(env[i] + 5);
			break ;
		}
	}
	if (!full_path)
		perror_exit("full_path");
	paths = ft_split(full_path, ':');
	free(full_path);
	return (paths);
}
