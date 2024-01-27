/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 12:09:07 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/27 12:34:56 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_echo(t_mini *data)
{
	(void)data;
    printf("echo not yet implemented\n");
}

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
		perror("cd");
}

void handle_pwd(t_mini *data)
{
	char	cwd[1024];

	(void)data;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		printf("pathname of current directory not found\n");
}

void handle_export(t_mini *data)
{
	(void)data;
    printf("export not yet implemented\n");
}

void handle_unset(t_mini *data)
{
	(void)data;
    printf("unset not yet implemented\n");
}

void handle_env(t_mini *data)
{
	char	**env;
	int		i;

	env = data->env;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
}

void handle_exit(t_mini *data)
{
	(void)data;
    exit(0);
}

void handle_tilde(t_mini *data)
{
	char	*tilde;

	tilde = data->cmds[0][0];
	if (path_exists(tilde))
		printf("bash: %s: Is a directory\n", tilde);
	else
		printf("bash: %s: No such file or directory\n", tilde);
	free(tilde);
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
}
