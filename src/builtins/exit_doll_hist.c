/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_doll_hist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:44:51 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/06 14:16:28 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit(t_mini *data)
{
	int i = -1;
	while (++i < data->seg_count)
		reset_redirections(&data->redir[i]);
	free(data->redir);
	reset_data_out(data);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

void	handle_doll(t_mini *data)
{
	(void)data;
	printf("%d : command not found\n", last_exit_status);
	last_exit_status = 127;
}

void	handle_hist(t_mini *data)
{
	int			i;
	HIST_ENTRY	**list_hist;

	(void)data;
	i = -1;
	list_hist = history_list();
	if (list_hist)
	{
		while (list_hist[++i])
			printf("%d: %s\n", i + 1, list_hist[i]->line);
	}
	last_exit_status = EXIT_SUCCESS;
}
