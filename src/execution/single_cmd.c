/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:55:38 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 11:37:13 by svolodin         ###   ########.fr       */
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

void	execute_single_command(t_mini *data, t_exec_cmd *exec_data, int i)
{
	pid_t	pid;
	char	*path;

	path = NULL;
	if (!is_builtin(data->cmds[i][0], data))
		path = find_path(data->paths, data->cmds[i]);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &ft_signal);
		signal(SIGQUIT, SIG_DFL);
		handle_input_redir(data, exec_data->pipe_end);
		handle_output_redir(data, exec_data->pipe_fds, i, exec_data->num_cmds);
		if (handle_builtin(data, i))
			exit(EXIT_SUCCESS);
		execve(path, data->cmds[i], data->env);
		free(path);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		exec_data->child_pids[i] = pid;
	else
		perror_exit("fork");
	if (path)
		free(path);
}
