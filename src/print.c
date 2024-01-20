/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:23:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/20 11:24:04 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(char **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
	{
		printf("Token %d: %s\n", i, tokens[i]);
		free(tokens[i]);
	}
	free(tokens);
}
