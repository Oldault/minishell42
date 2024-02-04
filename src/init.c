/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:50:21 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/04 12:17:32 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_env(char **env)
{
	int		env_vars;
	char	**new_env;
	int		i;

	env_vars = dbl_arr_len(env);
	new_env = (char **)malloc(sizeof(char *) * MAX_ENV_VARS);
	if (!new_env)
		return (perror("malloc failed"), NULL);
	i = -1;
	while (++i < env_vars)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
	}
	while(i < MAX_ENV_VARS)
		new_env[i++] = NULL;
	return (new_env);
}

static void	initialize_commands(t_mini *data)
{
	data->bltn = malloc(sizeof(cmd_entry_t) * 10);
	data->bltn[0] = (cmd_entry_t){"echo", handle_echo};
	data->bltn[1] = (cmd_entry_t){"cd", handle_cd};
	data->bltn[2] = (cmd_entry_t){"pwd", handle_pwd};
	data->bltn[3] = (cmd_entry_t){"export", handle_export};
	data->bltn[4] = (cmd_entry_t){"unset", handle_unset};
	data->bltn[5] = (cmd_entry_t){"env", handle_env};
	data->bltn[6] = (cmd_entry_t){"exit", handle_exit};
	data->bltn[7] = (cmd_entry_t){"history", handle_hist};
	data->bltn[8] = (cmd_entry_t){"$?", handle_doll};
	data->bltn[9] = (cmd_entry_t){NULL, NULL};
}

void	set_data_out(t_mini *data, char **env)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &ft_signal);
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
