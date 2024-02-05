/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:20:24 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 12:14:19 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_builtin(t_mini *data, char *cmd, t_cmd_entry *builtin)
{
	int	i;

	i = -1;
	while (builtin[++i].command_name != NULL)
	{
		if (ft_strcmp(builtin[i].command_name, cmd) == 0)
		{
			builtin[i].func(data);
			return (1);
		}
	}
	return (0);
}
