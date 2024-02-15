/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:44:09 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 09:49:05 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_heredoc(t_mini *data, char *lim)
{
	char	*line;
	size_t	len;

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
			break ;
		}
		write(data->in_fd, line, ft_strlen(line));
		write(data->in_fd, "\n", 1);
		free(line);
	}
	close(data->in_fd);
	data->in_fd = open(".hdoc.tmp", O_RDONLY, 0777);
	if (data->in_fd < 0)
		perror_exit("open heredoc file for reading");
}

static void	redir_input(t_mini *data, char *filename)
{
	int	new_fd;

	if (data->in_fd > 0 && data->in_fd != STDIN_FILENO)
	{
		close(data->in_fd);
		data->in_fd = -1;
	}
	new_fd = open(filename, O_RDONLY);
	if (new_fd < 0)
	{
		g_exit_stat = 1;
		perror("Failed to open input file");
		if (!data->err)
			data->err = ft_strdup("Failed to open input file: Bad address");
	}
	else
		data->in_fd = new_fd;
}

static void	redir_output(t_mini *data, char *filename)
{
	int	new_fd;

	if (data->out_fd > 0 && data->out_fd != STDOUT_FILENO)
	{
		close(data->out_fd);
		data->out_fd = -1;
	}
	new_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_fd < 0)
	{
		g_exit_stat = 1;
		perror("Failed to open output file");
		if (!data->err)
			data->err = ft_strdup("Failed to open output file: Bad address");
	}
	else
	{
		data->out_fd = new_fd;
	}
}

static void	redir_append(t_mini *data, char *filename)
{
	int	new_fd;

	if (data->out_fd > 0 && data->out_fd != STDOUT_FILENO)
	{
		close(data->out_fd);
		data->out_fd = -1;
	}
	new_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (new_fd < 0)
	{
		g_exit_stat = 1;
		perror("Failed to open append file");
		if (!data->err)
			data->err = ft_strdup("Failed to open append file: Bad address");
	}
	else
		data->out_fd = new_fd;
}

void	apply_redirections(t_mini *data, int cmd_index)
{
	t_redirs	redirs;
	t_rdr		redir;
	int			i;

	redirs = data->redir[cmd_index];
	i = -1;
	while (++i < redirs.count)
	{
		redir = redirs.redirs[i];
		if (redir.type == REDIR_INPUT)
			redir_input(data, redir.filename);
		else if (redir.type == REDIR_OUTPUT)
			redir_output(data, redir.filename);
		else if (redir.type == REDIR_APPEND)
			redir_append(data, redir.filename);
		else if (redir.type == REDIR_HEREDOC)
		{
			if (data->out_fd != STDOUT_FILENO)
				close(data->out_fd);
			data->in_fd = open(".hdoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			handle_heredoc(data, redir.filename);
			if (data->out_fd < 0)
				data->err = "open heredoc file : Bad adress";
		}
	}
}
