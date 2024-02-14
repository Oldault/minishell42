/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/14 21:51:57 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		last_exit_status = 0;

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
