/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:16:23 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/19 17:29:57 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PROMPT_SIZE 1024
#define PATH_SIZE 1024

static void	shorten_dir(const char *home_dir, const char *cwd, char *rel_cwd)
{
	if (home_dir && ft_strncmp(cwd, home_dir, ft_strlen(home_dir)) == 0)
	{
		rel_cwd[0] = '~';
		ft_strlcpy(rel_cwd + 1, cwd + ft_strlen(home_dir), PATH_SIZE - 1);
	}
	else
		ft_strlcpy(rel_cwd, cwd, PATH_SIZE);
}
char	*part2_prompt(char *prompt)
{
	char	*home_dir;
	char	cwd[PATH_SIZE];
	char	rel_cwd[PATH_SIZE];

	home_dir = getenv("HOME");
	getcwd(cwd, sizeof(cwd));
	shorten_dir(home_dir, cwd, rel_cwd);
	ft_strlcat(prompt, COLOR_BLUE, PROMPT_SIZE);
	ft_strlcat(prompt, rel_cwd, PROMPT_SIZE);
	ft_strlcat(prompt, "$ ", PROMPT_SIZE);
	ft_strlcat(prompt, COLOR_RESET, PROMPT_SIZE);
	return (prompt);
}

char	*part1_prompt(char *prompt)
{
	char	*username;
	char	*hostname;
	
	username = getenv("USER");
	if (username == NULL)
		username = getenv("LOGNAME");
	hostname = "minishell";
	ft_strlcat(prompt, COLOR_GREEN, PROMPT_SIZE);
	if (username)
		ft_strlcat(prompt, username, PROMPT_SIZE);
	ft_strlcat(prompt, "@", PROMPT_SIZE);
	ft_strlcat(prompt, hostname, PROMPT_SIZE);
	ft_strlcat(prompt, ":", PROMPT_SIZE);
	return (prompt);
}

char	*get_prompt(void)
{
	char	*prompt;

	prompt = malloc(PROMPT_SIZE * sizeof(char));
	if (prompt == NULL)
		return (NULL);
	prompt[0] = '\0';
	prompt = part1_prompt(prompt);
	prompt = part2_prompt(prompt);
	return (prompt);
}
