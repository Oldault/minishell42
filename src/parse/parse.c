/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:38:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/20 12:48:59 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dbl_arr_len(char **arr)
{
	int	count;

	count = 0;
	while (arr[count] != NULL)
		count++;
	return (count);
}

int	initialize_arrays(char ****cmd_arr, t_redirs **redir_arr, int seg_num)
{
	*cmd_arr = (char ***)malloc((seg_num + 1) * sizeof(char **));
	if (*cmd_arr == NULL)
		return (-1);
	*redir_arr = (t_redirs *)malloc((seg_num + 1) * sizeof(t_redirs));
	if (*redir_arr == NULL)
	{
		free(*cmd_arr);
		return (-1);
	}
	return (0);
}

void	free_resources(char **segments, char ***cmd_arr, t_redirs *redir_arr,
		int i)
{
	int	j;

	j = -1;
	free_double_array(segments);
	while (++j < i)
		free_double_array(cmd_arr[j]);
	free(cmd_arr);
	j = -1;
	while (++j <= i)
		reset_redirections(&redir_arr[j]);
	free(redir_arr);
}

int	process_segments(t_mini *data, char **segs, char ***cmds, t_redirs *r_arr)
{
	int	i;
	int	seg_num;

	i = -1;
	seg_num = data->seg_count;
	while (++i < seg_num)
	{
		cmds[i] = parse_segment(data, segs[i], &r_arr[i]);
		if (cmds[i] == NULL)
			return (free_resources(segs, cmds, r_arr, i), -1);
	}
	return (0);
}

int	parse(t_mini *data)
{
	t_redirs	*redir_arr;
	char		***cmd_arr;
	char		**segments;
	int			seg_num;

	if (is_only_spaces(data->input))
		return (0);
	segments = ft_split_quotes(data->input, '|');
	if (!segments)
		return (-1);
	seg_num = dbl_arr_len(segments);
	data->seg_count = seg_num;
	if (invalid_pipes(segments, seg_num, data))
		return (free_double_array(segments), -1);
	if (initialize_arrays(&cmd_arr, &redir_arr, seg_num) != 0)
		return (free_double_array(segments), -1);
	if (process_segments(data, segments, cmd_arr, redir_arr) != 0)
		return (free_resources(segments, cmd_arr, redir_arr, seg_num - 1), -1);
	cmd_arr[seg_num] = NULL;
	redir_arr[seg_num].redirs = NULL;
	redir_arr[seg_num].count = 0;
	data->cmds = cmd_arr;
	data->redir = redir_arr;
	free_double_array(segments);
	return (0);
}
