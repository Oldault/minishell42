/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 15:47:15 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*input;
	char	*prompt;
	char	**paths;
	char	***cmds;
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
		info.cmds = parse(info.input);
		handle_input(&info);
		free(info.input);
		free_cmds(&(info.cmds));
	}
	free_mini(&info);
	rl_clear_history();
	return (0);
}
