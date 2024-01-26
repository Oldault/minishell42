/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/26 12:31:14 by svolodin         ###   ########.fr       */
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

void apply_redirections(t_mini *info, int cmd_index)
{
    redirs_t redirs = info->redir[cmd_index];
    for (int i = 0; i < redirs.count; ++i)
    {
        redir_t redir = redirs.redirs[i];

        if (redir.type == REDIR_INPUT)
        {
            if (info->in_fd != STDIN_FILENO)
                close(info->in_fd);
            info->in_fd = open(redir.filename, O_RDONLY);
            if (info->in_fd < 0)
                perror_exit("open input file");
        }
        else if (redir.type == REDIR_OUTPUT)
        {
            if (info->out_fd != STDOUT_FILENO)
                close(info->out_fd);
            info->out_fd = open(redir.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (info->out_fd < 0)
                perror_exit("open output file");
        }
        else if (redir.type == REDIR_APPEND)
        {
            if (info->out_fd != STDOUT_FILENO)
                close(info->out_fd);
            info->out_fd = open(redir.filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (info->out_fd < 0)
                perror_exit("open append file");
        }
    }
}


void reset_file_descriptors(t_mini *info)
{
    if (info->in_fd != STDIN_FILENO)
    {
        close(info->in_fd);
        info->in_fd = STDIN_FILENO;
    }
    if (info->out_fd != STDOUT_FILENO)
    {
        close(info->out_fd);
        info->out_fd = STDOUT_FILENO;
    }
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
		apply_redirections(info, i);
		execute_single_command(info, pipe_end, pipe_fds, i, num_cmds);
		if (pipe_end != -1)
			close(pipe_end);
		if (i < num_cmds - 1)
		{
			pipe_end = pipe_fds[0];
			close(pipe_fds[1]);
		}
		else
			pipe_end = -1;
		reset_file_descriptors(info);
	}
	for (int i = 0; i < num_cmds; ++i)
		wait(NULL);
}
