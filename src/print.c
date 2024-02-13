/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:23:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 12:13:39 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	print_redir(t_rdr **redir_arr)
{
	int	i;

	i = -1;
	printf("%s------------\n", BLUE);
	while (redir_arr[++i])
	{
		if (redir_arr[i]->type == REDIR_INPUT)
			printf("%d: < %s\n", i, redir_arr[i]->filename);
		else if (redir_arr[i]->type == REDIR_OUTPUT)
			printf("%d: > %s\n", i, redir_arr[i]->filename);
		else if (redir_arr[i]->type == REDIR_APPEND)
			printf("%d: >> %s\n", i, redir_arr[i]->filename);
		else
			printf("%d: No redirection\n", i);
	}
	printf("------------%s\n", COLOR_RESET);
}

void	print_redir_blue(t_mini *data)
{
	t_rdr	*redir;

	printf("%s------------\n", BLUE);
	for (int i = 0; data->redir[i].redirs != NULL; i++)
	{
		printf("Command %d redirections:\n", i + 1);
		for (int j = 0; j < data->redir[i].count; j++)
		{
			redir = &data->redir[i].redirs[j];
			printf("Redirection type: ");
			switch (redir->type)
			{
			case REDIR_INPUT:
				printf("Input (<)");
				break ;
			case REDIR_OUTPUT:
				printf("Output (>)");
				break ;
			case REDIR_APPEND:
				printf("Append (>>)");
				break ;
			default:
				printf("None");
				break ;
			}
			printf(", Filename: %s\n", redir->filename);
		}
	}
	printf("------------%s\n", COLOR_RESET);
}
