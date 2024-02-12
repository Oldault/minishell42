/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/06 14:16:28 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_exit_status = 0;

int	main(int ac, char **av, char **env)
{
	t_mini	*data;

	(void)ac, (void)av;
	data = set_data_out(env);
	while (42)
	{
		set_data_in(data);
		if (!data->input)
		{
			reset_data_out(data);
			rl_clear_history(); 
			exit(EXIT_SUCCESS); 
		}
		if (strcmp(data->input, "") == 0)
		{
			free(data->input);
			continue ;
		}
		if (data->input)
			add_history(data->input);
		parse(data);
		//print_3d_arr(data->cmds, 1);
		// print_redir_blue(data);
		execute_commands(data);
		reset_data_in(data);
	}
	reset_data_out(data);
	rl_clear_history();
	return (0);
}
