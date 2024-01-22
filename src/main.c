/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/22 18:16:57 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmds(char ****cmds)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while ((*cmds)[++i] != NULL)
	{
		while ((*cmds)[i][++j] != NULL)
			free((*cmds)[i][j]);
		free((*cmds)[i]);
	}
	free(*cmds);
}

void	free_mini(t_mini *info)
{
	free(info->prompt);
	free(info->paths);
}

int	main(int ac, char **av, char **env)
{
	t_mini	info;

	(void)ac, (void)av;
	setup_signal_handlers();
	info.paths = get_paths(env);
	info.env = env;
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
		info.cmds = parse(info.input);
		handle_input(&info);
		free(info.input);
		free_cmds(&(info.cmds));
	}
	free_mini(&info);
	rl_clear_history();
	return (0);
}
