/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:34:25 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/14 15:46:02 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_only_spaces(const char *str)
{
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (!isspace((unsigned char)*str))
		{
			return (0);
		}
		str++;
	}
	return (1);
}

int	count_char_occurrences(const char *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == c)
		{
			count++;
		}
		str++;
	}
	return (count);
}

int	invalid_pipes(char **segments, int seg_num, t_mini *data)
{
	int	i;
	int	pipe_count;

	pipe_count = count_char_occurrences(data->input, '|');
	if (pipe_count > seg_num - 1)
	{
		ft_putstr_fd("minishell: syntax error with pipes\n", 2);
		return (1);
	}
	i = -1;
	while (++i < seg_num)
	{
		if (is_only_spaces(segments[i]))
		{
			ft_putstr_fd("minishell: syntax error with pipes\n", 2);
			return (1);
		}
	}
	return (0);
}
