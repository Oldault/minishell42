/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:20:24 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 15:40:26 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_exists(const char *path)
{
	struct stat	statbuf;

	return (stat(path, &statbuf) == 0);
}

char	*expand_tilde(const char *input)
{
	const char	*home_dir;
	char		*expanded_path;

	*home_dir = getenv("HOME");
	if (!home_dir)
		return (ft_strdup(input));
	if (input[0] != '~')
		return (ft_strdup(input));
	// If input is exactly "~", just return the home directory
	if (strcmp(input, "~") == 0)
		return (ft_strdup(home_dir));
	// For cases like "~/some/path
	*expanded_path = malloc(strlen(home_dir) + strlen(input)); // +1 for null terminator
	if (!expanded_path)
		return (NULL);
	strcpy(expanded_path, home_dir);
	strcat(expanded_path, input + 1); // Skip the tilde and concatenate
	return (expanded_path);
}

int	cd_command(const char *path)
{
	if (path == NULL || strcmp(path, "~") == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
			return (ft_putendl_fd("cd: HOME not set", 2), -1);
	}
	if (chdir(path) != 0)
		return (perror("cd"), -1);
	return (0);
}

void	handle_input(t_mini *info)
{
	char	*tilde;

	if (ft_strncmp(info->cmds[0][0], "history", 8) == 0)
		show_hist();
	else if (ft_strncmp(info->cmds[0][0], "exit", 5) == 0)
		exit(0);
	else if (ft_strncmp(info->cmds[0][0], "~", 1) == 0)
	{
		tilde = expand_tilde(info->cmds[0][0]);
		if (path_exists(tilde))
			printf("bash: %s: Is a directory\n", tilde);
		else
			printf("bash: %s: No such file or directory\n", tilde);
		free(tilde);
	}
	else if (ft_strncmp(info->cmds[0][0], "cd", 2) == 0)
	{
		if (info->cmds[0][1] == NULL)
			cd_command("~");
		else
			cd_command(info->cmds[0][1]);
	}
	else
		execute_commands(info);
}
