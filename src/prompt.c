/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:16:23 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/04 11:53:18 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PROMPT_SIZE 1024
#define PATH_SIZE 1024

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	char	*ptr;

	ptr = dest + ft_strlen(dest);
	while (*src != '\0' && n--)
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest;
	while (*src != '\0')
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

void	shorten_path(char *path, const char *home_dir)
{
	size_t	home_dir_length;
	size_t	path_length;

	path_length = 0;
	home_dir_length = 0;
	if (home_dir && ft_strncmp(path, home_dir, ft_strlen(home_dir)) == 0)
	{
		home_dir_length = ft_strlen(home_dir);
		path_length = ft_strlen(path);
	}
	if (home_dir_length == path_length || (home_dir_length + 1 == path_length
			&& path[home_dir_length] == '/'))
		ft_strcpy(path, "~");
	else
	{
		ft_memmove(path + 2, path + home_dir_length + 1, path_length
			- home_dir_length);
		path[0] = '~';
		path[1] = '/';
		path[path_length - home_dir_length + 1] = '\0';
	}
}

char	*get_prompt(void)
{
	char		*prompt;
	char		cwd[PATH_SIZE];
	const char	*home_dir;
	const char	*username;

	home_dir = getenv("HOME");
	prompt = malloc(PROMPT_SIZE);
	username = getenv("USER");
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("Unable to get current working directory");
		free(prompt);
		return (NULL);
	}
	if (username == NULL)
		username = "user";
	shorten_path(cwd, home_dir);
	prompt[0] = '\0';
	ft_strncat(prompt, username, PROMPT_SIZE - strlen(prompt) - 1);
	ft_strncat(prompt, "@minishell:", PROMPT_SIZE - strlen(prompt) - 1);
	ft_strncat(prompt, cwd, PROMPT_SIZE - strlen(prompt) - 1);
	ft_strncat(prompt, "$ ", PROMPT_SIZE - strlen(prompt) - 1);
	return (prompt);
}
