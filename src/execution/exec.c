/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/06 14:53:03 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_exit(char *str)
{
	perror(str);
	exit(127);
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
		unlink(".here_doc.tmp");
}

void	execute_commands(t_mini *data)
{
	int		num_cmds;
	pid_t	*child_pids;
	int		pipe_fds[2];
	int		pipe_end;
	char	*cmd_path;
		int status;

	if (data->cmds == NULL || data->cmds[0] == NULL || data->cmds[0][0] == NULL)
	{
		return ;
	}
	num_cmds = count_commands(data->cmds);
	if (num_cmds == 1 && handle_builtin(data, data->cmds[0][0], data->bltn))
	{
		return ;
	}
	child_pids = ft_calloc(num_cmds, sizeof(pid_t));
	if (!child_pids)
	{
		perror_exit("malloc");
	}
	pipe_end = -1;
	for (int i = 0; i < num_cmds; ++i)
	{
		cmd_path = find_path(data->paths, data->cmds[i]);
		if (cmd_path == NULL)
		{
			printf("%s: command not found\n", data->cmds[i][0]);
			free(child_pids);
			return ;
		}
		setup_pipes(&pipe_end, pipe_fds, i, num_cmds);
		apply_redirections(data, i);
		if (data->err != NULL)
		{
			printf("%s\n", data->err);
		}
		else
		{
			execute_single_command(data, pipe_end, pipe_fds, i, num_cmds,
				child_pids);
		}
		free(cmd_path); // Free cmd_path after each command execution
		if (pipe_end != -1)
		{
			close(pipe_end);
		}
		if (i < num_cmds - 1)
		{
			pipe_end = pipe_fds[0];
			close(pipe_fds[1]);
		}
		else
		{
			pipe_end = -1;
		}
		reset_file_descriptors(data, i);
	}
	for (int i = 0; i < num_cmds; ++i)
	{
		waitpid(child_pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			last_exit_status = WEXITSTATUS(status);
		}
		else
		{
			last_exit_status = 127;
		}
	}
	free(child_pids); // Ensure child_pids is freed after use
}
