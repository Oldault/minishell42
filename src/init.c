/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:50:21 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 13:17:10 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_env(char **env)
{
	int		env_vars;
	char	**new_env;
	int		i;

	env_vars = dbl_arr_len(env);
	new_env = (char **)ft_calloc(MAX_ENV_VARS, sizeof(char *));
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
	return (new_env);
}

static void	initialize_commands(t_mini *data)
{
	data->bltn = ft_calloc(10, sizeof(t_cmd_entry));
	
	data->bltn[0] = (t_cmd_entry){"echo", handle_echo};
	data->bltn[1] = (t_cmd_entry){"cd", handle_cd};
	data->bltn[2] = (t_cmd_entry){"pwd", handle_pwd};
	data->bltn[3] = (t_cmd_entry){"export", handle_export};
	data->bltn[4] = (t_cmd_entry){"unset", handle_unset};
	data->bltn[5] = (t_cmd_entry){"env", handle_env};
	data->bltn[6] = (t_cmd_entry){"exit", handle_exit};
	data->bltn[7] = (t_cmd_entry){"history", handle_hist};
	data->bltn[8] = (t_cmd_entry){"$?", handle_doll};
	data->bltn[9] = (t_cmd_entry){NULL, NULL};
}

void	set_data_out(t_mini *data, char **env)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &ft_signal);
	data->env = get_env(env);
	initialize_commands(data);
	data->redir = ft_calloc(1, sizeof(t_redirs));
	if (!data->redir) {
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	data->redir->redirs = NULL;
	data->redir->count = 0;
}

void reset_redirections(t_redirs *redir)
{
    if (redir != NULL)
	{
        for (int i = 0; i < redir->count; i++) {
            free(redir->redirs[i].filename);
        }
        free(redir->redirs);
        redir->redirs = NULL;
        redir->count = 0;
    }
}

void	set_data_in(t_mini *data)
{
	reset_redirections(data->redir);
	data->in_fd = STDIN_FILENO;
	data->out_fd = STDOUT_FILENO;
	data->err = NULL;
	data->cmds = NULL;
	data->prompt = get_prompt();
	data->input = readline(data->prompt);
	data->paths = get_paths(data, data->env);
}
