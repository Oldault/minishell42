/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 15:41:40 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_mini	info;

	(void)ac, (void)av;
	setup_signal_handlers();
	info.paths = get_paths(env);
	info.env = env;
	info.cmds = NULL;
	info.in_fd = STDIN_FILENO;
	info.out_fd = STDOUT_FILENO;
	while (42)
	{
		info.prompt = get_prompt();
		info.input = readline(info.prompt);
		if (info.input == NULL)
		{
			free(info.input);
			continue ;
		}
		if (*info.input)
			add_history(info.input);
		info.cmds = parse(&info);
		//print_3d_arr(info.cmds);
		handle_input(&info);
		free(info.input);
		free_cmds(&(info.cmds));
	}
	free_mini(&info);
	rl_clear_history();
	return (0);
}
