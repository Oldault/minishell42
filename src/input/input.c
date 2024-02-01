/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:20:24 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/01 11:48:13 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(t_mini *data)
{
	char	*cmd;
	int		i;
	
	if (data->cmds == NULL || data->cmds[0] == NULL || data->cmds[0][0] == NULL)
		return ;

	i = -1;
	cmd = data->cmds[0][0];
	while (data->builtin_cmds[++i].command_name != NULL)
	{
		if (ft_strcmp(data->builtin_cmds[i].command_name, cmd) == 0)
		{
			data->builtin_cmds[i].func(data);
			return ;
		}
	}
	execute_commands(data);
}
