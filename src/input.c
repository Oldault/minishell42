/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:20:24 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/20 16:12:40 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_env(char **env)
{
	int	i;

	i = -1;
	while (env[++i] != NULL)
		printf("%s\n", env[i]);
}

char	*get_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (ft_strdup(cwd));
	else
		return (NULL);
}

int	path_exists(const char *path)
{
	struct stat	statbuf;

	return (stat(path, &statbuf) == 0);
}

char	*expand_tilde(const char *input)
{
	char		*expanded_path;
	const char	*home_dir;

	if (input[0] != '~')
		return (ft_strdup(input));
	home_dir = getenv("HOME");
	if (!home_dir)
		return (ft_strdup(input));
	expanded_path = malloc(strlen(home_dir) + strlen(input));
	if (!expanded_path)
		return (NULL);
	strcpy(expanded_path, home_dir);
	strcat(expanded_path, input + 1);
	return (expanded_path);
}

int	cd_command(const char *path)
{
	if (path == NULL || strcmp(path, "~") == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (-1);
		}
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (-1);
	}
	return (0);
}

void	print_echo(char *arg)
{
	int		newline;

	newline = 1;
	if (arg != NULL && strcmp(arg, "-n") == 0)
	{
		newline = 0;
		arg = strtok(NULL, " ");
	}
	while (arg != NULL)
	{
		printf("%s", arg);
		arg = strtok(NULL, " ");
		if (arg != NULL)
			printf(" ");
	}
	if (newline)
		printf("\n");
}

void	handle_input(char *input, char **env)
{
	char	*pwd;
	char	*tilde;
	char	*command;
	char	*arg;

	command = strtok(input, " ");
	(void)command;
	arg = strtok(NULL, " ");
	if (ft_strncmp(input, "history", 8) == 0)
		show_hist();
	else if (ft_strncmp(input, "exit", 5) == 0)
	{
		free(input);
		exit(0);
	}
	else if (ft_strncmp(input, "env", 4) == 0)
		execute_env(env);
	else if (ft_strncmp(input, "pwd", 4) == 0)
	{
		pwd = get_pwd();
		printf("%s\n", pwd);
		free(pwd);
	}
	else if (ft_strncmp(input, "~", 1) == 0)
	{
		tilde = expand_tilde(input);
		if (path_exists(tilde))
			printf("bash: %s: Is a directory\n", tilde);
		else
			printf("bash: %s: No such file or directory\n", tilde);
		free(tilde);
	}
	else if (ft_strncmp(input, "cd", 2) == 0)
	{
		if (arg == NULL)
			ft_putendl_fd("cd: missing argument", 2);
		else
			cd_command(arg);
	}
	else if (ft_strncmp(input, "echo", 4) == 0)
		print_echo(arg);
}
