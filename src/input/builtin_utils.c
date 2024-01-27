/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 12:25:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/27 12:26:53 by svolodin         ###   ########.fr       */
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
	expanded_path = malloc(strlen(home_dir) + ft_strlen(input));
	if (!expanded_path)
		return (NULL);
	strcpy(expanded_path, home_dir);
	strcat(expanded_path, input + 1);
	return (expanded_path);
}

int	path_exists(const char *path)
{
	struct stat	statbuf;

	return (stat(path, &statbuf) == 0);
}
