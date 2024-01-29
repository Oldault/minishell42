/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/29 17:21:22 by svolodin         ###   ########.fr       */
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

void	execute_single_command(t_mini *data, int pipe_end, int *pipe_fds, int i,
		int num_cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
        if (data->in_fd != STDIN_FILENO)
		{
            dup2(data->in_fd, STDIN_FILENO);
            close(data->in_fd);
        } else if (pipe_end != -1)
		{
            dup2(pipe_end, STDIN_FILENO);
            close(pipe_end);
        }
        if (data->out_fd != STDOUT_FILENO)
		{
            dup2(data->out_fd, STDOUT_FILENO);
            close(data->out_fd);
        } else if (i < num_cmds - 1)
		{
            dup2(pipe_fds[1], STDOUT_FILENO);
            close(pipe_fds[0]);
            close(pipe_fds[1]);
        }
		execve(find_path(data->paths, data->cmds[i]), data->cmds[i], data->env);
		printf("%s: command not found\n", data->cmds[i][0]);
		exit(127);
	}
	else if (pid < 0)
		perror_exit("fork");
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            last_exit_status = WEXITSTATUS(status);
        } else {
            last_exit_status = 127;
        }
    }
}

void	handle_heredoc(t_mini *data, char *lim)
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
        write(data->in_fd, line, ft_strlen(line));
        write(data->in_fd, "\n", 1);
        free(line);
	}
	close(data->in_fd);
	data->in_fd = open(".here_doc.tmp", O_RDONLY, 0777);
    if (data->in_fd < 0)
        perror_exit("open heredoc file for reading");
}

// todo STOP EXITING PROGRAM IF INCORRECT FILE
void apply_redirections(t_mini *data, int cmd_index)
{
    redirs_t redirs;
    redir_t redir;
    
    redirs = data->redir[cmd_index];
    for (int i = 0; i < redirs.count; ++i)
    {
        redir = redirs.redirs[i];
        if (redir.type == REDIR_INPUT)
        {
            if (data->in_fd != STDIN_FILENO)
                close(data->in_fd);
            data->in_fd = open(redir.filename, O_RDONLY);
            if (data->in_fd < 0)
                data->err = "open input file : Bad adress";
        }
        else if (redir.type == REDIR_OUTPUT)
        {
            if (data->out_fd != STDOUT_FILENO)
                close(data->out_fd);
            data->out_fd = open(redir.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (data->out_fd < 0)
                data->err = "open output file : Bad adress";
        }
        else if (redir.type == REDIR_APPEND)
        {
            if (data->out_fd != STDOUT_FILENO)
                close(data->out_fd);
            data->out_fd = open(redir.filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (data->out_fd < 0)
                data->err = "open append file : Bad adress";
        }
        else if (redir.type == REDIR_HEREDOC)
        {
            if (data->out_fd != STDOUT_FILENO)
                close(data->out_fd);
            data->in_fd = open(".here_doc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            handle_heredoc(data, redir.filename);
            if (data->out_fd < 0)
                data->err = "open heredoc file : Bad adress";
        }
    }
}

void reset_file_descriptors(t_mini *data, int i)
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
    if (data->redir->redirs[i].type == REDIR_HEREDOC)
        unlink(".here_doc.tmp");
}

void	execute_commands(t_mini *data)
{
	int	num_cmds;
	int	pipe_fds[2];
	int	pipe_end;

	num_cmds = count_commands(data->cmds);
	pipe_end = -1;
	for (int i = 0; i < num_cmds; ++i)
	{
		setup_pipes(&pipe_end, pipe_fds, i, num_cmds);
		apply_redirections(data, i);
        if (data->err != NULL)
            printf("%s\n", data->err);
        else
		    execute_single_command(data, pipe_end, pipe_fds, i, num_cmds);
		if (pipe_end != -1)
			close(pipe_end);
		if (i < num_cmds - 1)
		{
			pipe_end = pipe_fds[0];
			close(pipe_fds[1]);
		}
		else
			pipe_end = -1;
		reset_file_descriptors(data, i);
	}
	for (int i = 0; i < num_cmds; ++i)
		wait(NULL);
}
