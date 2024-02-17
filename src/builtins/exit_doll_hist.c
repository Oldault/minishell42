/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_doll_hist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:44:51 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 09:48:22 by svolodin         ###   ########.fr       */
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

void	handle_exit(t_mini *data, char **args)
{
	int	i;
	int	exit_status;

	exit_status = validate_exit_args(args);
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

void	handle_doll(t_mini *data)
{
	(void)data;
	printf("%d : command not found\n", g_exit_stat);
	g_exit_stat = 127;
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
	g_exit_stat = EXIT_SUCCESS;
}
