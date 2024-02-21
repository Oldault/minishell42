/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:11:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 12:17:25 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd(t_mini *data, int cmd_index)
{
	char	*path;

	path = data->cmds[cmd_index][1];
	if (!path || ft_strcmp(path, "~") == 0)
	{
		path = ft_getenv(data->env, "HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			g_exit_stat = EXIT_FAILURE;
			return ;
		}
		chdir(path);
		free(path);
	}
	else if (chdir(path) != 0)
	{
		ft_putendl_fd("cd: no such file or directory", 2);
		g_exit_stat = EXIT_FAILURE;
		return ;
	}
	g_exit_stat = EXIT_SUCCESS;
}

void	handle_pwd(t_mini *data, int cmd_index)
{
	char	cwd[1024];

	(void)data;
	(void)cmd_index;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		g_exit_stat = EXIT_SUCCESS;
	}
	else
	{
		printf("pathname of current directory not found\n");
		exit(127);
	}
}

void	handle_env(t_mini *data, int cmd_index)
{
	char	**env;
	int		i;

	(void)cmd_index;
	env = data->env;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	g_exit_stat = EXIT_SUCCESS;
}
