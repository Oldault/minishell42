/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:16:23 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 11:33:33 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PROMPT_SIZE 1024
#define PATH_SIZE 1024

void	shorten_path(char *path, char *home_dir)
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

void	prepare_cwd(char *cwd, char *home_dir, char *prompt)
{
	if (getcwd(cwd, PATH_SIZE) == NULL)
	{
		perror("Unable to get current working directory");
		free(prompt);
		cwd[0] = '\0';
	}
	else
	{
		shorten_path(cwd, home_dir);
	}
}

void	construct_prompt(char *prompt, char *username, char *cwd)
{
	prompt[0] = '\0';
	ft_strncat(prompt, username, PROMPT_SIZE - ft_strlen(prompt) - 1);
	ft_strncat(prompt, "@minishell:", PROMPT_SIZE - ft_strlen(prompt) - 1);
	ft_strncat(prompt, cwd, PROMPT_SIZE - ft_strlen(prompt) - 1);
	ft_strncat(prompt, "$ ", PROMPT_SIZE - ft_strlen(prompt) - 1);
}

char	*fetch_env_value(char **env, char *var_name)
{
	char	*value;

	value = ft_getenv(env, var_name);
	if (!value)
	{
		if (ft_strcmp(var_name, "USER") == 0)
		{
			return (ft_strdup("user"));
		}
	}
	return (value);
}

char	*get_prompt(char **env)
{
	char	*prompt;
	char	cwd[PATH_SIZE];
	char	*home_dir;
	char	*username;

	home_dir = fetch_env_value(env, "HOME");
	username = fetch_env_value(env, "USER");
	prompt = malloc(PROMPT_SIZE);
	if (!prompt)
	{
		if (home_dir)
			free(home_dir);
		if (username)
			free(username);
		return (NULL);
	}
	prepare_cwd(cwd, home_dir, prompt);
	if (home_dir)
		free(home_dir);
	construct_prompt(prompt, username, cwd);
	free(username);
	return (prompt);
}
