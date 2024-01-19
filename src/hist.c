/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albeninc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:43:44 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/19 15:06:53 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void	show_hist(void)
{
	int			i;
	HIST_ENTRY	**list_hist;

	i = 0;
	list_hist = history_list();
	if (list_hist)
	{
		while (list_hist[i])
		{
			printf("%d: %s\n", i + 1, list_hist[i]->line);
			++i;
		}
	}
}

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("Minishell :> ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		if (strcmp(input, "history") == 0)
			show_hist();
		free(input);
	}
	return (0);
}
