/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:55:38 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/30 11:55:50 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_input_redir(t_mini *data, int pipe_end)
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

static void handle_output_redir(t_mini *data, int *pipe_fds, int i, int num_cmds)
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

static void handle_parent_process(pid_t pid)
{
    if (pid < 0)
    {
        perror_exit("fork");
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            last_exit_status = WEXITSTATUS(status);
        else
            last_exit_status = 127;
    }
}


void	execute_single_command(t_mini *data, int pipe_end, int *pipe_fds, int i,
		int num_cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
        handle_input_redir(data, pipe_end);
        handle_output_redir(data, pipe_fds, i, num_cmds);
		execve(find_path(data->paths, data->cmds[i]), data->cmds[i], data->env);
		printf("%s: command not found\n", data->cmds[i][0]);
		exit(127);
	}
    else
		handle_parent_process(pid);
}
