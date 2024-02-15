/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 12:25:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 12:39:54 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_exists(const char *path)
{
	struct stat	statbuf;

	return (stat(path, &statbuf) == 0);
}

char	*strdup_spc(const char *src)
{
	const char	*end;
	char		*dest;
	size_t		length;
	size_t		i;

	end = src;
	while (*end != ' ' && *end != '\0')
	{
		end++;
	}
	length = end - src;
	dest = (char *)malloc(length + 1);
	if (dest == NULL)
		return (perror("malloc failed"), NULL);
	i = -1;
	while (++i < length)
	{
		dest[i] = src[i];
	}
	dest[length] = '\0';
	return (dest);
}

char	*strdup_alpha(const char *src)
{
	const char	*end = src;
	size_t		length;
	size_t		i;
	char		*dest;

	while (*end && ft_isalpha(*end))
	{
		end++;
	}
	length = end - src;
	dest = (char *)malloc(length + 1);
	if (dest == NULL)
	{
		perror("Failed to allocate memory");
		return (NULL);
	}
	i = -1;
	while (++i < length)
	{
		dest[i] = src[i];
	}
	dest[length] = '\0';
	return (dest);
}
