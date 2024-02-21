/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:44:09 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 13:54:16 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_heredoc(t_mini *data, int going, char *lim)
{
	close(data->in_fd);
	data->in_fd = open(".hdoc.tmp", O_RDONLY, 0777);
	if (!going && g_exit_stat != 130)
	{
		ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(lim, 2);
		ft_putendl_fd("`)", 2);
		g_exit_stat = 0;
	}
	if (data->in_fd < 0)
		perror_exit("open heredoc file for reading");
}

void	handle_heredoc(t_mini *data, char *lim)
{
	char	*line;
	int		going;
	size_t	len;

	line = NULL;
	while (42)
	{
		line = readline("> ");
		if (!line)
			break ;
		going = 1;
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
		going = 0;
	}
	exit_heredoc(data, going, lim);
}

int	apply_redirections(t_mini *data, int cmd_index)
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
			redir_heredoc(data, redir.filename);
	}
	return (0);
}
