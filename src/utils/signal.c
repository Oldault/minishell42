/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:49:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 12:29:24 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal_fork(int num)
{
	if (num == SIGINT)
	{
		exit(130);
	}
	if (num == SIGQUIT)
		exit(131);
}

void	ft_signal(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_stat = 130;
	}
}

// void	ft_signal_heredoc(int signal)
// {
// 	if (signal == SIGINT)
// 	{
// 		close(STDIN_FILENO);
// 		write(STDERR_FILENO, "\n", 1);
// 	}
// }
