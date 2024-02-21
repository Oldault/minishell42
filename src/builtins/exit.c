/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:44:51 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 12:24:54 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_num(const char *str)
{
	if (*str == '-')
		++str;
	while (*str)
	{
		if (!ft_isdigit((unsigned char)*str))
			return (0);
		++str;
	}
	return (1);
}

int	validate_exit_args(char **args)
{
	if (args[1])
	{
		if (is_num(args[1]))
		{
			if (args[2])
			{
				ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
				return (-1);
			}
			else
				return (ft_atoi(args[1]));
		}
		else
		{
			ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
			return (2);
		}
	}
	return (EXIT_SUCCESS);
}

void	handle_exit(t_mini *data, int cmd_index)
{
	int	i;
	int	exit_status;

	exit_status = validate_exit_args(data->cmds[cmd_index]);
	if (exit_status == -1)
	{
		g_exit_stat = 1;
		return ;
	}
	i = -1;
	while (++i < data->seg_count)
		reset_redirections(&data->redir[i]);
	free(data->redir);
	reset_data_out(data);
	rl_clear_history();
	g_exit_stat = exit_status;
	exit(g_exit_stat);
}
