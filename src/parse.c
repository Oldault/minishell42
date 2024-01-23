/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:38:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 08:32:35 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	***parse(t_mini *info)
{
	int		i;
	int		j;
	char	**segments;
	char	***arr;

	segments = ft_split(info->input, '|');
	if (segments == NULL)
		return (NULL);
	i = 0;
	while (segments[i])
		i++;
	arr = (char ***)malloc((i + 1) * sizeof(char **));
	if (arr == NULL)
		return (free_double_array(segments), NULL);
	j = -1;
	while (++j < i)
	{
		redirect(segments[j], info);
		printf("After parse: in_fd = %d, out_fd = %d\n", info->in_fd, info->out_fd);
		arr[j] = ft_split(segments[j], ' ');
		if (arr[j] == NULL)
			return (free(segments[j]), free_triple_array(arr), NULL);
	}
	free_double_array(segments);
	arr[j] = NULL;
	return (arr);
}
