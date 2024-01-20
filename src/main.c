/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/20 13:02:03 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char				*input;
	char				*prompt;
	char				**tokens;

	(void)ac;
	(void)av;
	setup_signal_handlers();
	prompt = get_prompt();
	while (42)
	{
		input = readline(prompt);
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		handle_input(input, env);
		tokens = custom_tokenize(input);
		//print_tokens(tokens);
		free(input);
	}
	free(prompt);
	rl_clear_history();
	return (0);
}
