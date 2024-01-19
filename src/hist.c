/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:43:44 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/19 15:18:23 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_hist(void)
{
	int			i;
	HIST_ENTRY	**list_hist;

	i = -1;
	list_hist = history_list();
	if (list_hist)
	{
		while (list_hist[++i])
			printf("%d: %s\n", i + 1, list_hist[i]->line);
	}
}
