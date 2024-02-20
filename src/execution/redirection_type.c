/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_type.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:49:27 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 15:52:38 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_input(t_mini *data, char *filename)
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

void	redir_output(t_mini *data, char *filename)
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

void	redir_append(t_mini *data, char *filename)
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

int	redir_heredoc(t_mini *data, char *filename)
{
	int	stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
		return (perror("dup"), -1);
	if (data->out_fd != STDOUT_FILENO)
		close(data->out_fd);
	data->in_fd = open(".hdoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	signal(SIGINT, &ft_signal_heredoc);
	handle_heredoc(data, filename);
	signal(SIGINT, &ft_signal);
	if (dup2(stdin_copy, STDIN_FILENO) == -1)
		return (perror("dup2"), -1);
	close(stdin_copy);
	if (data->out_fd < 0)
		data->err = ft_strdup("open heredoc file : Bad address");
	return (0);
}
