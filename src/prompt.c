/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:16:23 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 06:49:41 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PROMPT_SIZE 1024
#define PATH_SIZE 1024

void	shorten_path(char *path, const char *home_dir)
{
	int	tot_len;

	tot_len = ft_strlen(path) - ft_strlen(home_dir) + 1;
	if (home_dir && ft_strncmp(path, home_dir, ft_strlen(home_dir)) == 0)
	{
		ft_memmove(path, path + ft_strlen(home_dir), tot_len);
		path[0] = '~';
	}
}

char	*get_prompt(void)
{
	char		*prompt;
	char		cwd[PATH_SIZE];
	const char	*home_dir;
	const char	*username;

	username = getenv("USER");
	home_dir = getenv("HOME");
	prompt = malloc(PROMPT_SIZE);
	if (prompt == NULL)
		return (perror("prompt"), NULL);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("cwd"), free(prompt), NULL);
	if (username == NULL)
		username = "user";
	shorten_path(cwd, home_dir);
	prompt[0] = '\0';
	strncat(prompt, username, PROMPT_SIZE - strlen(prompt) - 1);
	strncat(prompt, "@minishell:", PROMPT_SIZE - strlen(prompt) - 1);
	strncat(prompt, cwd, PROMPT_SIZE - strlen(prompt) - 1);
	strncat(prompt, "$ ", PROMPT_SIZE - strlen(prompt) - 1);
	return (prompt);
}
