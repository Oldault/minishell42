/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:20:24 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/29 18:39:49 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef void	(*t_cmd_func)(t_mini *);
typedef struct
{
    char		*command_name;
    t_cmd_func	func;
}				cmd_entry_t;

cmd_entry_t	cmds[] = {
	{"cd", handle_cd},
	{"pwd", handle_pwd},
	{"export", handle_export},
	{"unset", handle_unset},
	{"env", handle_env},
	{"exit", handle_exit},
	{"~", handle_tilde},
	{"history", handle_hist},
	{"$?", handle_doll},
	{NULL, NULL}
};

void	handle_input(t_mini *data)
{
	char	*cmd;
	int		i;
	
	if (data->cmds == NULL || data->cmds[0] == NULL || data->cmds[0][0] == NULL)
		return ;

	i = -1;
	cmd = data->cmds[0][0];
	while (cmds[++i].command_name != NULL)
	{
		if (ft_strcmp(cmds[i].command_name, cmd) == 0)
		{
			cmds[i].func(data);
			return ;
		}
	}
	execute_commands(data);
}

