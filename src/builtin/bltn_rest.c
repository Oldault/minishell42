/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_rest.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:44:51 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 17:51:13 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_cd(t_mini *data)
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

void handle_pwd(t_mini *data)
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

void handle_env(t_mini *data)
{
	char	**env;
	int		i;

	env = data->env;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	last_exit_status = EXIT_SUCCESS;
}

void handle_exit(t_mini *data)
{
	free_mini(data);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

void handle_doll(t_mini *data)
{
	(void)data;
	printf("%d : command not found\n", last_exit_status);
    last_exit_status = 127;
}

void	handle_hist(t_mini *data)
{
	int			i;
	HIST_ENTRY	**list_hist;

	(void)data;
	i = -1;
	list_hist = history_list();
	if (list_hist)
	{
		while (list_hist[++i])
			printf("%d: %s\n", i + 1, list_hist[i]->line);
	}
	last_exit_status = EXIT_SUCCESS;
}
