/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 17:15:05 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_file_descriptors(t_mini *data, int i)
{
	if (data->in_fd != STDIN_FILENO)
	{
		close(data->in_fd);
		data->in_fd = STDIN_FILENO;
	}
	if (data->out_fd != STDOUT_FILENO)
	{
		close(data->out_fd);
		data->out_fd = STDOUT_FILENO;
	}
	if (data->redir != NULL && data->redir->redirs[i].type == REDIR_HEREDOC)
		unlink(".hdoc.tmp");
}

void	handle_wait(t_exec_cmd *exec_data, int i)
{
	int	status;
	int	termsig;

	status = 0;
	signal(SIGINT, SIG_IGN);
	if (waitpid(exec_data->child_pids[i], &status, 0) > 0)
	{
		if (WIFEXITED(status))
			g_exit_stat = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			termsig = WTERMSIG(status);
			if (termsig == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			g_exit_stat = 128 + termsig;
		}
		else
			g_exit_stat = 127;
	}
	signal(SIGINT, &ft_signal);
	signal(SIGQUIT, SIG_IGN);
}

int	exec_cmd_seg(t_mini *data, t_exec_cmd *exec_data, int i)
{
	if (!is_builtin(data->cmds[i][0], data))
	{
		exec_data->cmd_path = find_path(data, data->paths, data->cmds[i]);
		handle_cmd_path(data->cmds[i][0], exec_data->cmd_path, exec_data);
	}
	setup_pipes(exec_data->pipe_fds, i, exec_data->num_cmds);
	apply_redirections(data, i);
	execute_single_command(data, exec_data, i);
	if (!is_builtin(data->cmds[i][0], data))
		free(exec_data->cmd_path);
	if (exec_data->pipe_end != -1)
		close(exec_data->pipe_end);
	if (i < exec_data->num_cmds - 1)
	{
		exec_data->pipe_end = exec_data->pipe_fds[0];
		close(exec_data->pipe_fds[1]);
	}
	else
		exec_data->pipe_end = -1;
	reset_file_descriptors(data, i);
	return (1);
}

int	execute_commands(t_mini *data)
{
	t_exec_cmd	*exec_data;
	int			i;

	if (data->cmds == NULL || data->cmds[0] == NULL || data->cmds[0][0] == NULL)
	{
		if (data->redir->redirs->type == REDIR_HEREDOC)
			apply_redirections(data, 0);
		return (0);
	}
	exec_data = init_exec_data(data);
	if (exec_data->num_cmds == 1 && handle_builtin(data, 0))
		return (free(exec_data->child_pids), free(exec_data), -1);
	i = -1;
	while (++i < exec_data->num_cmds)
		exec_cmd_seg(data, exec_data, i);
	i = -1;
	while (++i < exec_data->num_cmds)
		handle_wait(exec_data, i);
	free(exec_data->child_pids);
	free(exec_data);
	return (1);
}

int	is_builtin(char *cmd, t_mini *data)
{
	int	i;

	i = 0;
	while (data->bltn[i].command_name != NULL)
	{
		if (ft_strcmp(cmd, data->bltn[i].command_name) == 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
}
