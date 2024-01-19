/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/19 16:16:36 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;
	char	*prompt;

	prompt = get_prompt();
	while (42)
	{
		input = readline(prompt);
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		if (strcmp(input, "history") == 0)
			show_hist();
		free(input);
	}
	free(prompt);
	return (0);
}
