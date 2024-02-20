/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:44:09 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 15:50:08 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
