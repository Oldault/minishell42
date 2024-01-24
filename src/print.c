/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:23:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/24 15:03:28 by svolodin         ###   ########.fr       */
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

void	print_3d_arr(char ***arr, int clr)
{
	int	i;

	i = -1;
	if (clr == 1)
		printf("%s------------\n", RED);
	else if (clr == 2)
		printf("%s------------\n", GREEN);
	else if (clr == 3)
		printf("%s------------\n", BLUE);
	else
		printf("------------\n");
	while (arr[++i])
	{
		ft_printf("%d: ", i);
		print_2d_arr(arr[i], ',');
	}
	printf("------------%s\n", COLOR_RESET);
}

void print_redir(Redirection **redir_arr)
{
    if (redir_arr == NULL)
    {
        printf("\033[34mNo redirections\033[0m\n");
        return;
    }

    int i = 0;
	printf("%s------------\n", BLUE);
    while (redir_arr[i])
    {
        switch (redir_arr[i]->type)
        {
            case REDIR_INPUT:
                printf("%d: < %s\n", i, redir_arr[i]->filename);
                break;
            case REDIR_OUTPUT:
                printf("%d: > %s\n", i, redir_arr[i]->filename);
                break;
            case REDIR_APPEND:
                printf("%d: >> %s\n", i, redir_arr[i]->filename);
                break;
            default:
                printf("%d: No redirection\n", i);
                break;
        }
        i++;
    }
	printf("------------%s\n", COLOR_RESET);
}
