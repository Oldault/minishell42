/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 13:00:10 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_stat = 0;

void	core_loop(t_mini *data)
{
	while (42)
	{
		set_data_in(data);
		if (!data->input)
		{
			reset_data_out(data);
			rl_clear_history();
			exit(EXIT_SUCCESS);
			continue ;
		}
		if (ft_strcmp(data->input, "") == 0)
		{
			reset_data_mid(data);
			continue ;
		}
		if (data->input)
			add_history(data->input);
		if (parse(data) != 0)
		{
			reset_data_mid(data);
			continue ;
		}
		//print_3d_arr(data->cmds, 1);
		//print_redir_blue(data);
		execute_commands(data);
		reset_data_in(data);
	}
}

int	main(int ac, char **av, char **env)
{
	t_mini	*data;

	(void)ac;
	(void)av;
	data = set_data_out(env);
	core_loop(data);
	reset_data_out(data);
	rl_clear_history();
	return (0);
}
