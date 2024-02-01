/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:50:21 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/01 17:27:18 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_count(char **array)
{
	int	count;

	count = 0;
	while (*array)
	{
		count++;
		array++;
	}
	return (count);
}

static char	**get_env(char **env)
{
	int		env_vars;
	char	**new_env;
	int		i;

	env_vars = str_count(env);
	new_env = (char **)malloc(sizeof(char *) * (env_vars + 1));
	i = -1;
	while (++i < env_vars)
		new_env[i] = ft_strdup(env[i]);
	new_env[i] = NULL;
	return (new_env);
}

static void	initialize_commands(t_mini *data)
{
	data->bltn = malloc(sizeof(cmd_entry_t) * 5);
	data->bltn_fork = malloc(sizeof(cmd_entry_t) * 7);

	data->bltn[0] = (cmd_entry_t){"cd", handle_cd};
	data->bltn[1] = (cmd_entry_t){"export", handle_export};
	data->bltn[2] = (cmd_entry_t){"unset", handle_unset};
	data->bltn[3] = (cmd_entry_t){"exit", handle_exit};
	data->bltn[4] = (cmd_entry_t){NULL, NULL};

	data->bltn_fork[0] = (cmd_entry_t){"echo", handle_echo};
	data->bltn_fork[1] = (cmd_entry_t){"pwd", handle_pwd};
	data->bltn_fork[2] = (cmd_entry_t){"env", handle_env};
	data->bltn_fork[3] = (cmd_entry_t){"~", handle_tilde};
	data->bltn_fork[4] = (cmd_entry_t){"history", handle_hist};
	data->bltn_fork[5] = (cmd_entry_t){"$?", handle_doll};
	data->bltn_fork[6] = (cmd_entry_t){NULL, NULL};
}

void	set_data_out(t_mini *data, char **env)
{
	data->env = get_env(env);
	initialize_commands(data);
}

void	set_data_in(t_mini *data)
{
	data->in_fd = STDIN_FILENO;
	data->out_fd = STDOUT_FILENO;
	data->err = NULL;
	data->cmds = NULL;
	data->redir = NULL;
	data->prompt = get_prompt();
	data->input = readline(data->prompt);
	data->paths = get_paths(data, data->env);
}
