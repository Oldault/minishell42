/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 12:09:07 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/31 17:05:01 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_echo(t_mini *data)
{
	(void)data;
    printf("echo not yet implemented\n");
}

void handle_cd(t_mini *data)
{
	char	*path;

	path = data->cmds[0][1];
	if (path == NULL || strcmp(path, "~") == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
			ft_putendl_fd("cd: HOME not set", 2);
	}
	if (chdir(path) != 0)
	{
		printf("cd : no such file or directory\n");
		last_exit_status = EXIT_FAILURE;
		return ;
	}
	last_exit_status = EXIT_SUCCESS;
}

void handle_pwd(t_mini *data)
{
	char	cwd[1024];

	(void)data;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		last_exit_status = EXIT_SUCCESS;
	}
	else
	{
		printf("pathname of current directory not found\n");
		exit(127);
	}
}

void handle_export(t_mini *data)
{
	char	*input = data->input + 7;
    const char *delimiter = strchr(input, '=');

    if (delimiter != NULL)
	{
        size_t name_length = delimiter - input;
		
        char *name = malloc(name_length + 1);
        if (name == NULL) {
            perror("malloc failed");
            return;
        }
        strncpy(name, input, name_length);
        name[name_length] = '\0';
        const char *value = delimiter + 1;
        setenv(name, value, 1);
        free(name);
    } else {
        printf("Usage: export NAME=VALUE\n");
    }
}

void handle_unset(t_mini *data)
{
	char	*to_unset;
	char	**env;
	char	*temp;
	int		i;

	to_unset = data->cmds[0][1];
	env = data->env;
	i = -1;
	while (env[++i] != NULL)
	{
		temp = (ft_split(env[i], '='))[0];
		if (ft_strcmp(temp, to_unset) == 0)
		{
			free(env[i]);
			free(temp);
			while (env[i] != NULL)
			{
				env[i] = env[i + 1];
				i++;
			}
			break ;
		}
		free(temp);
	}
}

void handle_env(t_mini *data)
{
	char	**env;
	int		i;

	env = data->env;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	last_exit_status = EXIT_SUCCESS;
}

void handle_exit(t_mini *data)
{
	(void)data;
    exit(0);
}

void handle_doll(t_mini *data)
{
	(void)data;
	printf("%d : command not found\n", last_exit_status);
    last_exit_status = 127;
}

void handle_tilde(t_mini *data)
{
	char	*tilde;

	tilde = data->cmds[0][0];
	if (path_exists(tilde))
		printf("bash: %s: Is a directory\n", tilde);
	else
		printf("bash: %s: No such file or directory\n", tilde);
	free(tilde);
	last_exit_status = EXIT_SUCCESS;
}

void	handle_hist(t_mini *data)
{
	int			i;
	HIST_ENTRY	**list_hist;

	(void)data;
	i = -1;
	list_hist = history_list();
	if (list_hist)
	{
		while (list_hist[++i])
			printf("%d: %s\n", i + 1, list_hist[i]->line);
	}
	last_exit_status = EXIT_SUCCESS;
}
