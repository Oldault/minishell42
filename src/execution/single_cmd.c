/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:55:38 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 12:28:14 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input_redir(t_mini *data, int pipe_end)
{
	if (data->in_fd != STDIN_FILENO)
	{
		dup2(data->in_fd, STDIN_FILENO);
		close(data->in_fd);
	}
	else if (pipe_end != -1)
	{
		dup2(pipe_end, STDIN_FILENO);
		close(pipe_end);
	}
}

static void	handle_output_redir(t_mini *data, int *pipe_fds, int i,
		int num_cmds)
{
	if (data->out_fd != STDOUT_FILENO)
	{
		dup2(data->out_fd, STDOUT_FILENO);
		close(data->out_fd);
	}
	else if (i < num_cmds - 1)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

void	execute_single_command(t_mini *data, int pipe_end, int *pipe_fds, int i,
		int num_cmds, pid_t *child_pids)
{
	pid_t pid;
	char *path = find_path(data->paths, data->cmds[i]);

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &ft_signal_fork);
		signal(SIGQUIT, &ft_signal_fork);
		handle_input_redir(data, pipe_end);
		handle_output_redir(data, pipe_fds, i, num_cmds);
		if (handle_builtin(data, data->cmds[i][0], data->bltn))
		{
			free(path);
			exit(EXIT_SUCCESS);
		}
		execve(path, data->cmds[i], data->env);
		perror("execve");
		free(path);
		exit(127);
	}
	else if (pid > 0)
	{
		child_pids[i] = pid;
	}
	else
	{
		
		perror_exit("fork");
	}
	free(path);
}