/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/21 14:38:49 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*input;
	char	*prompt;
	char	**tokens;
	char	**paths;

	(void)ac;
	(void)av;
	setup_signal_handlers();
	paths = get_paths(env);
	//print_2d_arr(paths, ',');
	while (42)
	{
		prompt = get_prompt();
		input = readline(prompt);
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		handle_input(input, env, paths);
		tokens = tokenizer(input);
		print_tokens(tokens);
		free(input);
	}
	free(prompt);
	rl_clear_history();
	return (0);
}
