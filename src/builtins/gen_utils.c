/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 12:25:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 11:30:52 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_tilde(const char *input)
{
	const char	*home_dir;
	char		*expanded_path;

	home_dir = getenv("HOME");
	if (!home_dir)
		return (ft_strdup(input));
	if (input[0] != '~')
		return (ft_strdup(input));
	if (strcmp(input, "~") == 0)
		return (ft_strdup(home_dir));
	expanded_path = malloc(ft_strlen(home_dir) + ft_strlen(input));
	if (!expanded_path)
		return (NULL);
	ft_strcpy(expanded_path, home_dir);
	strcat(expanded_path, input + 1);
	return (expanded_path);
}

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
