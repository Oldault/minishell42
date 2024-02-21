/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_type.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:49:27 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 13:48:43 by svolodin         ###   ########.fr       */
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
