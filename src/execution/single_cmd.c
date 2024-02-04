/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:55:38 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/04 15:50:39 by svolodin         ###   ########.fr       */
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

void	execute_single_command(t_mini *data, int pipe_end, int *pipe_fds, int i, int num_cmds, pid_t *child_pids)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
        signal(SIGINT, &ft_signal_fork);
		signal(SIGQUIT, &ft_signal_fork);
        handle_input_redir(data, pipe_end);
        handle_output_redir(data, pipe_fds, i, num_cmds);
        if (handle_builtin(data, data->cmds[i][0], data->bltn))
            exit(EXIT_SUCCESS);
        execve(find_path(data->paths, data->cmds[i]), data->cmds[i], data->env);
        printf("%s: command not found\n", data->cmds[i][0]);
        exit(127);
    }
    else if (pid > 0)
        child_pids[i] = pid;
    else
        perror_exit("fork");
    signal(SIGINT, SIG_IGN);
}
