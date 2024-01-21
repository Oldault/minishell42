/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:23:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/21 14:12:03 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(char **tokens)
{
	int	i;

	i = -1;
	printf("%s------------\n", RED);
	while (tokens[++i])
	{
		printf("Token %d: %s\n", i, tokens[i]);
		free(tokens[i]);
	}
	printf("------------%s\n", COLOR_RESET);
	free(tokens);
}

void	print_2d_arr(char **arr, char separator)
{
	int	i;

	i = -1;
	ft_putchar_fd('[', 1);
	while (arr[++i])
	{
		ft_printf("'%s'", arr[i]);
		if (arr[i + 1])
			ft_putchar_fd(separator, 1);
	}
	ft_putstr_fd("]\n", 1);
}

void	print_3d_arr(char ***arr)
{
	int	i;

	i = -1;
	while (arr[++i])
	{
		ft_printf("%d: ", i);
		print_2d_arr(arr[i], ',');
		ft_printf("\n");
	}
}
