/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/30 11:18:02 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int last_exit_status = 0;

void	set_data_out(t_mini	*data, char **env)
{
	data->paths = get_paths(data, env);
	data->env = env;
}

void	set_data_in(t_mini	*data)
{
	data->in_fd = STDIN_FILENO;
	data->out_fd = STDOUT_FILENO;
	data->err = NULL;
	data->cmds = NULL;
	data->redir = NULL;
	data->prompt = get_prompt();
	data->input = readline(data->prompt);
}

int	main(int ac, char **av, char **env)
{
	t_mini	data;

	(void)ac, (void)av;
	setup_signal_handlers();
	set_data_out(&data, env);
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
    	parse(&data);
		//print_3d_arr(data.cmds, 1);
		//print_redir_blue(&data);
		handle_input(&data);
		free(data.input);
		free_cmds(&(data.cmds));
	}
	free_mini(&data);
	rl_clear_history();
	return (0);
}
