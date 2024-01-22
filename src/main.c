/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/22 16:18:34 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*input;
	char	*prompt;
	char	**paths;
	char	***cmds;

	(void)ac;
	(void)av;
	setup_signal_handlers();
	paths = get_paths(env);
	//print_2d_arr(paths, ',');
	while (42)
	{
		prompt = get_prompt();
		input = readline(prompt);
		if (!input)
			return (-1); //todo handle
		if (*input)
			add_history(input);
		cmds = parse(input);
		//print_3d_arr(cmds);
		handle_input(cmds, env, paths);
		free(input);
	}
	free(prompt);
	rl_clear_history();
	return (0);
}
