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

	(void)ac;
	(void)av;
	setup_signal_handlers();
	paths = get_paths(env);
	last_exit_status = 0;
	last_command_was_dollar = 0;
	//print_2d_arr(paths, ',');
	while (42)
	{
		prompt = get_prompt();
		input = readline(prompt);
		if (!input)
			return (-1); //todo handle
		if (strcmp(input, "") == 0)
		{
			free(input); // Free the allocated empty string
			continue ;
		}
		if (*input)
			add_history(input);
		if (!last_command_was_dollar)
		{
			last_exit_status = 0;
		}
		if (do_signal(input, &last_command_was_dollar, &last_exit_status))
		{
			continue ;
		}
		cmds = parse(input);
		//print_3d_arr(cmds);
		handle_input(cmds, env, paths);
		free(input);
	}
	free(prompt);
	rl_clear_history();
	return (0);
}
