/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:20:24 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/20 11:20:34 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void execute_env(char **env)
{
	int	i;

	i = -1;
    while (env[++i] != NULL)
        printf("%s\n", env[i]);
}

void	handle_input(char *input, char **env)
{
	if (ft_strncmp(input, "history", 8) == 0)
			show_hist();
	else if (ft_strncmp(input, "exit", 5) == 0)
	{
		free(input);
		exit(0);
	}
	else if (ft_strncmp(input, "env", 4) == 0)
		execute_env(env);
}
