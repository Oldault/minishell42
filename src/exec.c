/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 17:02:16 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

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
			perror_exit("pipe");
	}
}

void	execute_single_command(t_mini *info, int pipe_end, int *pipe_fds, int i,
		int num_cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		//printf("Child process: in_fd = %d, out_fd = %d, pipe_end = %d\n", info->in_fd, info->out_fd, pipe_end);
        if (info->in_fd != STDIN_FILENO)
		{
            dup2(info->in_fd, STDIN_FILENO);
            close(info->in_fd);
        } else if (pipe_end != -1)
		{
            dup2(pipe_end, STDIN_FILENO);
            close(pipe_end);
        }
        if (info->out_fd != STDOUT_FILENO)
		{
            dup2(info->out_fd, STDOUT_FILENO);
            close(info->out_fd);
        } else if (i < num_cmds - 1)
		{
            dup2(pipe_fds[1], STDOUT_FILENO);
            close(pipe_fds[0]);
            close(pipe_fds[1]);
        }
		//printf("in_fd: %d\nout_fd: %d\n", info->in_fd, info->out_fd);
		execve(find_path(info->paths, info->cmds[i]), info->cmds[i], info->env);
		printf("%s: command not found\n", info->cmds[i][0]);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror_exit("fork");
}

void	execute_commands(t_mini *info)
{
	int	num_cmds;
	int	pipe_fds[2];
	int	pipe_end;

	num_cmds = count_commands(info->cmds);
	pipe_end = -1;
	for (int i = 0; i < num_cmds; ++i)
	{
		setup_pipes(&pipe_end, pipe_fds, i, num_cmds);
		execute_single_command(info, pipe_end, pipe_fds, i, num_cmds);
		//printf("After command execution: pipe_end = %d, pipe_fds[0] = %d, pipe_fds[1] = %d\n", pipe_end, pipe_fds[0], pipe_fds[1]);
		if (pipe_end != -1)
			close(pipe_end);
		if (i < num_cmds - 1)
		{
			pipe_end = pipe_fds[0];
			close(pipe_fds[1]);
		}
		else
			pipe_end = -1;
	}
	for (int i = 0; i < num_cmds; ++i)
		wait(NULL);
}
