/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:38:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/22 20:27:40 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	***parse(char *str)
{
	int		i;
	int		j;
	char	**segments;
	char	***arr;

	segments = ft_split(str, '|');
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
		arr[j] = ft_split(segments[j], ' ');
		if (arr[j] == NULL)
			return (free(segments[j]), free_triple_array(arr), NULL);
	}
	free_double_array(segments);
	arr[j] = NULL;
	return (arr);
}
