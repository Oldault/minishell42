/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:21:32 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 12:15:19 by svolodin         ###   ########.fr       */
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

t_exec_cmd	*init_exec_data(t_mini *data)
{
	t_exec_cmd	*exec_data;

	exec_data = ft_calloc(1, sizeof(t_exec_cmd));
	if (!exec_data)
		return (NULL);
	exec_data->num_cmds = count_commands(data->cmds);
	exec_data->pipe_end = -1;
	exec_data->child_pids = ft_calloc(exec_data->num_cmds, sizeof(pid_t));
	if (!exec_data->child_pids)
	{
		free(exec_data);
		perror_exit("malloc");
	}
	exec_data->cmd_path = NULL;
	return (exec_data);
}

void	setup_pipes(int *pipe_fds, int i, int num_cmds)
{
	if (i < num_cmds - 1)
	{
		if (pipe(pipe_fds) < 0)
			perror_exit("pipe");
	}
}

int	handle_cmd_path(char *cmd, char *cmd_path, t_exec_cmd *exec_data)
{
	if (cmd_path == NULL)
	{
		printf("%s: command not found\n", cmd);
		g_exit_stat = 127;
		free(exec_data->child_pids);
		free(exec_data);
		return (0);
	}
	if (access(cmd_path, F_OK) == 0)
	{
		if (access(cmd_path, X_OK) != 0)
		{
			printf("%s: permission denied\n", cmd_path);
			g_exit_stat = 126;
			free(exec_data->cmd_path);
			free(exec_data->child_pids);
			free(exec_data);
			return (0);
		}
	}
	return (1);
}

int	handle_builtin(t_mini *data, int cmd_index)
{
	int			i;
	char		*cmd;
	t_cmd_entry	*builtin;

	i = -1;
	builtin = data->bltn;
	cmd = data->cmds[cmd_index][0];
	while (builtin[++i].command_name != NULL)
	{
		if (ft_strcmp(builtin[i].command_name, cmd) == 0)
		{
			builtin[i].func(data, cmd_index);
			return (1);
		}
	}
	return (0);
}
