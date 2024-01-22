/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/22 10:30:19 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_commands(char ***cmds)
{
	int	num_cmds;

	num_cmds = 0;
	while (cmds[num_cmds])
		num_cmds++;
	return (num_cmds);
}

static void	setup_pipes(int *pipe_end, int *pipe_fds, int i, int num_cmds)
{
	(void)pipe_end;
	if (i < num_cmds - 1)
	{
		if (pipe(pipe_fds) < 0)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
}

void	execute_single_command(char **cmd, int pipe_end, int *pipe_fds, int i,
		int num_cmds, char **env, char **paths)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (pipe_end != -1)
		{
			dup2(pipe_end, STDIN_FILENO);
			close(pipe_end);
		}
		if (i < num_cmds - 1)
		{
			dup2(pipe_fds[1], STDOUT_FILENO);
			close(pipe_fds[0]);
			close(pipe_fds[1]);
		}
		execve(find_path(paths, cmd), cmd, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	execute_commands(char ***cmds, char **env, char **paths)
{
	int	num_cmds;
	int	pipe_fds[2];
	int	pipe_end;

	num_cmds = count_commands(cmds);
	pipe_end = -1;
	for (int i = 0; i < num_cmds; ++i)
	{
		setup_pipes(&pipe_end, pipe_fds, i, num_cmds);
		execute_single_command(cmds[i], pipe_end, pipe_fds, i, num_cmds, env,
			paths);
		if (pipe_end != -1)
			close(pipe_end);
		if (i < num_cmds - 1)
		{
			pipe_end = pipe_fds[0];
			close(pipe_fds[1]);
		}
		else
			pipe_end = -1;
		if (i < num_cmds - 1)
			close(pipe_fds[1]);
	}
	for (int i = 0; i < num_cmds; ++i)
		wait(NULL);
}
