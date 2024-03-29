/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:23:47 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 11:50:26 by svolodin         ###   ########.fr       */
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

void	print_redir_type(t_rdr *redir)
{
	printf("Redirection type: ");
	if (redir->type == REDIR_INPUT)
		printf("Input (<)");
	else if (redir->type == REDIR_OUTPUT)
		printf("Output (>)");
	else if (redir->type == REDIR_APPEND)
		printf("Append (>>)");
	else if (redir->type == REDIR_HEREDOC)
		printf("Heredoc (<<)");
	else
		printf("None");
	printf(", Filename: %s\n", redir->filename);
}

void	print_redir_blue(t_mini *data)
{
	t_rdr	*redir;
	int		i;
	int		j;

	j = -1;
	i = -1;
	printf("%s------------\n", BLUE);
	while (data->redir[++i].redirs != NULL)
	{
		printf("Command %d redirections:\n", i + 1);
		while (++j < data->redir[i].count)
		{
			redir = &data->redir[i].redirs[j];
			print_redir_type(redir);
		}
	}
	printf("------------%s\n", COLOR_RESET);
}
