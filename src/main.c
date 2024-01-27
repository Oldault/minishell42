/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/27 11:47:24 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_data_out(t_mini	*data, char **env)
{
	data->paths = get_paths(env);
	data->env = env;
}

void	set_data_in(t_mini	*data)
{
	data->in_fd = STDIN_FILENO;
	data->out_fd = STDOUT_FILENO;
	data->cmds = NULL;
	data->redir = NULL;
	data->prompt = get_prompt();
	data->input = readline(data->prompt);
}

int	main(int ac, char **av, char **env)
{

	int		lst_cmd_dlr;
	int		lst_ext_stat;
	t_mini	data;

	(void)ac, (void)av;
	setup_signal_handlers();
	set_data_out(&data, env);
 	lst_ext_stat = 0;
	lst_cmd_dlr = 0;
	while (42)
	{
		set_data_in(&data);
		if (!data.input)
			return (-1);
		if (strcmp(data.input, "") == 0)
		{
			free(data.input);
			continue ;
		}
		if (*(data.input))
			add_history(data.input);
		if (!lst_cmd_dlr)
			lst_ext_stat = 0;
		if (do_signal(data.input, &lst_cmd_dlr, &lst_ext_stat))
			continue ;
    	parse(&data);
		//print_3d_arr(data.cmds, 1);
		//print_redir_blue(&data);
		handle_input(&data);
		free(data.input);
		//free_cmds(&(data.cmds));
	}
	free_mini(&data);
	rl_clear_history();
	return (0);
}
