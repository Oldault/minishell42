/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:20:24 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/01 12:04:35 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_builtin(t_mini *data, char *cmd)
{
	int		i;

	i = -1;
	while (data->builtin_cmds[++i].command_name != NULL)
	{
		if (ft_strcmp(data->builtin_cmds[i].command_name, cmd) == 0)
		{
			data->builtin_cmds[i].func(data);
			return (1);
		}
	}
	return (0);
}
