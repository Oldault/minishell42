/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/26 16:19:29 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_info_out(t_mini	*info, char **env)
{
	info->paths = get_paths(env);
	info->env = env;
}

void	set_info_in(t_mini	*info)
{
	info->in_fd = STDIN_FILENO;
	info->out_fd = STDOUT_FILENO;
	info->cmds = NULL;
	info->redir = NULL;
	info->prompt = get_prompt();
	info->input = readline(info->prompt);
}

int	main(int ac, char **av, char **env)
{

	int		lst_cmd_dlr;
	int		lst_ext_stat;
	t_mini	info;

	(void)ac, (void)av;
	setup_signal_handlers();
	set_info_out(&info, env);
 	lst_ext_stat = 0;
	lst_cmd_dlr = 0;
	while (42)
	{
		set_info_in(&info);
		if (!info.input)
			return (-1);
		if (strcmp(info.input, "") == 0)
		{
			free(info.input);
			continue ;
		}
		if (*(info.input))
			add_history(info.input);
		if (!lst_cmd_dlr)
			lst_ext_stat = 0;
		if (do_signal(info.input, &lst_cmd_dlr, &lst_ext_stat))
			continue ;
    	parse(&info);
		//print_3d_arr(info.cmds, 1);
		//print_redir_blue(&info);
		handle_input(&info);
		free(info.input);
		//free_cmds(&(info.cmds));
	}
	free_mini(&info);
	rl_clear_history();
	return (0);
}
