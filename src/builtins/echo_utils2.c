/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 10:14:49 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 10:15:01 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_dollar_only(char *name)
{
	free(name);
	return (ft_strdup("$"));
}

char	*handle_exit_status(char *name)
{
	free(name);
	return (ft_itoa(g_exit_stat));
}
