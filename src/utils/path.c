/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:08:13 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 17:16:47 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(t_mini *data, char **paths, char **arg)
{
	int		i;
	char	*path;
	char	*temp;

	path = NULL;
	i = 0;
	if (arg[0][0] == '/' || arg[0][0] == '.')
		return (ft_strdup(arg[0]));
	if (!paths || is_builtin(arg[0], data))
		return (NULL);
	while (paths[i])
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
		i++;
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

char	*ft_getenv(char **env, char *name)
{
	int		i;
	char	*temp_value;
	char	**temp;

	i = -1;
	while (env[++i])
	{
		temp = ft_split(env[i], '=');
		if (ft_strcmp(temp[0], name) == 0)
		{
			temp_value = ft_strdup(temp[1]);
			free_double_array(temp);
			return (temp_value);
		}
		free_double_array(temp);
	}
	return (NULL);
}
