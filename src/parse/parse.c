/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:38:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/26 17:36:37 by svolodin         ###   ########.fr       */
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

int	initialize_arrays(char ****cmd_arr, redirs_t **redir_arr, int seg_num)
{
	*cmd_arr = (char ***)malloc((seg_num + 1) * sizeof(char **));
	*redir_arr = (redirs_t *)malloc((seg_num + 1) * sizeof(redirs_t));
	if (*cmd_arr == NULL || *redir_arr == NULL)
	{
		if (*cmd_arr != NULL)
			free(*cmd_arr);
		if (*redir_arr != NULL)
			free(*redir_arr);
		return (-1);
	}
	return (0);
}

void	free_resources(char **segments, char ***cmd_arr, redirs_t *redir_arr,
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
		free_redir_array(&redir_arr[j]);
	free(redir_arr);
}

int	process_segments(char **segs, char ***cmds, redirs_t *r_arr, int seg_num)
{
	int	i;

	i = -1;
	while (++i < seg_num)
	{
		cmds[i] = parse_segment(segs[i], &r_arr[i]);
		if (cmds[i] == NULL)
			return (free_resources(segs, cmds, r_arr, i), -1);
	}
	return (0);
}

int	parse(t_mini *info)
{
	redirs_t	*redir_arr;
	char		***cmd_arr;
	char		**segments;
	int			seg_num;

	segments = ft_split(info->input, '|');
	if (segments == NULL)
		return (-1);
	seg_num = dbl_arr_len(segments);
	if (initialize_arrays(&cmd_arr, &redir_arr, seg_num) != 0)
		return (free_double_array(segments), -1);
	if (process_segments(segments, cmd_arr, redir_arr, seg_num) != 0)
		return (-1);
	cmd_arr[seg_num] = NULL;
	redir_arr[seg_num].redirs = NULL;
	redir_arr[seg_num].count = 0;
	info->cmds = cmd_arr;
	info->redir = redir_arr;
	free_double_array(segments);
	return (0);
}
