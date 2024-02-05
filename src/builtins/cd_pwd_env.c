/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:11:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 19:19:05 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd(t_mini *data)
{
	char	*path;

	path = data->cmds[0][1];
	if (path == NULL || strcmp(path, "~") == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
			ft_putendl_fd("cd: HOME not set", 2);
	}
	if (chdir(path) != 0)
	{
		printf("cd : no such file or directory\n");
		last_exit_status = EXIT_FAILURE;
		return ;
	}
	last_exit_status = EXIT_SUCCESS;
}

void	handle_pwd(t_mini *data)
{
	char	cwd[1024];

	(void)data;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		last_exit_status = EXIT_SUCCESS;
	}
	else
	{
		printf("pathname of current directory not found\n");
		exit(127);
	}
}

void	handle_env(t_mini *data)
{
	char	**env;
	int		i;

	env = data->env;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	last_exit_status = EXIT_SUCCESS;
}
