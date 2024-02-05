/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:44:09 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 12:13:55 by svolodin         ###   ########.fr       */
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
	data->in_fd = open(".here_doc.tmp", O_RDONLY, 0777);
	if (data->in_fd < 0)
		perror_exit("open heredoc file for reading");
}

static void	redir_input(t_mini *data, char *filename)
{
	if (data->in_fd != STDIN_FILENO)
		close(data->in_fd);
	data->in_fd = open(filename, O_RDONLY);
	if (data->in_fd < 0)
		data->err = "open input file : Bad adress";
}

static void	redir_output(t_mini *data, char *filename)
{
	if (data->out_fd != STDOUT_FILENO)
		close(data->out_fd);
	data->out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->out_fd < 0)
		data->err = "open output file : Bad adress";
}

static void	redir_append(t_mini *data, char *filename)
{
	if (data->out_fd != STDOUT_FILENO)
		close(data->out_fd);
	data->out_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->out_fd < 0)
		data->err = "open append file : Bad adress";
}

void	apply_redirections(t_mini *data, int cmd_index)
{
	t_redirs	redirs;
	t_rdr		redir;

	redirs = data->redir[cmd_index];
	for (int i = 0; i < redirs.count; ++i)
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
			data->in_fd = open(".here_doc.tmp", O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			handle_heredoc(data, redir.filename);
			if (data->out_fd < 0)
				data->err = "open heredoc file : Bad adress";
		}
	}
}
