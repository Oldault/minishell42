/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/06 12:03:33 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_exit_status = 0;

int	main(int ac, char **av, char **env)
{
	t_mini	*data;

	(void)ac, (void)av;
	data = malloc(sizeof(t_mini));
	if (data == NULL)
		return (EXIT_FAILURE);
	*data = (t_mini){0};
	set_data_out(data, env);
	while (42)
	{
		set_data_in(data);
		if (!data->input)
			break ;
		if (strcmp(data->input, "") == 0)
		{
			free(data->input);
			continue ;
		}
		if (data->input)
			add_history(data->input);
		parse(data);
		//print_3d_arr(data->cmds, 1);
		//print_redir_blue(data);
		execute_commands(data);
	}
	free_mini(data);
	rl_clear_history();
	return (0);
}
