/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:34:25 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 12:52:47 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_only_spaces(const char *str)
{
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str != ' ' && *str != '\f' && *str != '\n' && *str != '\r'
			&& *str != '\t' && *str != '\v')
			return (0);
		str++;
	}
	return (1);
}

int	count_char_occurrences(const char *str, char c)
{
	int	count;
	int	in_quote;

	in_quote = 0;
	count = 0;
	while (*str)
	{
		if (*str == '\"')
		{
			in_quote = !in_quote;
		}
		else if (*str == c && !in_quote)
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

	if (is_only_spaces(data->input))
		return (0);
	pipe_count = count_char_occurrences(data->input, '|');
	if (pipe_count > seg_num - 1)
	{
		ft_putendl_fd("minishell: syntax error with pipes", 2);
		g_exit_stat = 2;
		return (1);
	}
	i = -1;
	while (++i < seg_num)
	{
		if (is_only_spaces(segments[i]))
		{
			ft_putendl_fd("minishell: syntax error with pipes", 2);
			g_exit_stat = 2;
			return (1);
		}
	}
	return (0);
}

int	echo_pipe(t_mini *data)
{
	if (ft_strncmp(data->input, "echo ", 5) == 0)
	{
		data->cmds = malloc(sizeof(char **) * 2);
		data->cmds[0] = malloc(sizeof(char *) * 3);
		data->cmds[0][0] = ft_strdup("echo");
		data->cmds[0][1] = ft_strdup(data->input + 5);
		data->cmds[0][2] = NULL;
		data->cmds[1] = NULL;
		return (1);
	}
	return (0);
}
