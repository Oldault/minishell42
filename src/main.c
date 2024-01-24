/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/24 12:05:20 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{

	int		last_command_was_dollar;
	int		last_exit_status;
	t_mini	info;

	(void)ac, (void)av;
	setup_signal_handlers();
	info.paths = get_paths(env);
	info.env = env;
 	last_exit_status = 0;
	last_command_was_dollar = 0;
	while (42)
	{
		info.in_fd = STDIN_FILENO;
		info.out_fd = STDOUT_FILENO;
		info.cmds = NULL;
		info.prompt = get_prompt();
		info.input = readline(info.prompt);
		if (!info.input)
			return (-1); //todo handle
		if (strcmp(info.input, "") == 0)
		{
			free(info.input); // Free the allocated empty string
			continue ;
		}
		if (*(info.input))
			add_history(info.input);
		if (!last_command_was_dollar)
		{
			last_exit_status = 0;
		}
		if (do_signal(info.input, &last_command_was_dollar, &last_exit_status))
		{
			continue ;
		}
    info.cmds = parse(&info);
		//print_3d_arr(info.cmds);
		handle_input(&info);
		free(info.input);
		//free_cmds(&(info.cmds));
	}
	free_mini(&info);
	rl_clear_history();
	return (0);
}
