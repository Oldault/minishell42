/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/26 18:38:09 by svolodin         ###   ########.fr       */
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
		execve(find_path(info->paths, info->cmds[i]), info->cmds[i], info->env);
		printf("%s: command not found\n", info->cmds[i][0]);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror_exit("fork");
}

void	handle_heredoc(t_mini *info, char *lim)
{
	char	*line;
    size_t  len;

	line = NULL;
	while (42)
	{
		line = readline("> ");
        if (!line)
            break ;
        len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        if (ft_strcmp(line, lim) == 0)
        {
            free(line);
            break;
        }
        write(info->in_fd, line, ft_strlen(line));
        write(info->in_fd, "\n", 1);
        free(line);
	}
	close(info->in_fd);
	info->in_fd = open(".here_doc.tmp", O_RDONLY, 0777);
    if (info->in_fd < 0)
        perror_exit("open heredoc file for reading");
}

// todo STOP EXITING PROGRAM IF INCORRECT FILE
void apply_redirections(t_mini *info, int cmd_index)
{
    redirs_t redirs;
    redir_t redir;
    
    redirs = info->redir[cmd_index];
    for (int i = 0; i < redirs.count; ++i)
    {
        redir = redirs.redirs[i];
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
        else if (redir.type == REDIR_HEREDOC)
        {
            if (info->out_fd != STDOUT_FILENO)
                close(info->out_fd);
            info->in_fd = open(".here_doc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            handle_heredoc(info, redir.filename);
            if (info->out_fd < 0)
                perror_exit("open heredoc file");
        }
    }
}

void reset_file_descriptors(t_mini *info, int i)
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
    if (info->redir->redirs[i].type == REDIR_HEREDOC)
        unlink(".here_doc.tmp");
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
		reset_file_descriptors(info, i);
	}
	for (int i = 0; i < num_cmds; ++i)
		wait(NULL);
}
