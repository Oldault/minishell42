/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/01 11:46:31 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int last_exit_status = 0;

int str_count(char **array)
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

char	**get_env(char **env)
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

void	initialize_commands(t_mini *data)
{
    data->builtin_cmds = malloc(sizeof(cmd_entry_t) * 11);

    data->builtin_cmds[0] = (cmd_entry_t){"echo", handle_echo};
    data->builtin_cmds[1] = (cmd_entry_t){"cd", handle_cd};
    data->builtin_cmds[2] = (cmd_entry_t){"pwd", handle_pwd};
    data->builtin_cmds[3] = (cmd_entry_t){"export", handle_export};
    data->builtin_cmds[4] = (cmd_entry_t){"unset", handle_unset};
    data->builtin_cmds[5] = (cmd_entry_t){"env", handle_env};
    data->builtin_cmds[6] = (cmd_entry_t){"exit", handle_exit};
    data->builtin_cmds[7] = (cmd_entry_t){"~", handle_tilde};
    data->builtin_cmds[8] = (cmd_entry_t){"history", handle_hist};
    data->builtin_cmds[9] = (cmd_entry_t){"$?", handle_doll};
    data->builtin_cmds[10] = (cmd_entry_t){NULL, NULL};
}

void	set_data_out(t_mini	*data, char **env)
{
	data->env = get_env(env);
	initialize_commands(data);
}

void	set_data_in(t_mini	*data)
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

int	main(int ac, char **av, char **env)
{
	t_mini	data;

	(void)ac, (void)av;
	setup_signal_handlers();
	set_data_out(&data, env);
	while (42)
	{
		set_data_in(&data);
		if (!data.input)
			return (-1);
		if (strcmp(data.input, "") == 0)
		{
			free(data.input);
			continue ;
		}
		if (*(data.input))
			add_history(data.input);
    	parse(&data);
		//print_3d_arr(data.cmds, 1);
		//print_redir_blue(&data);
		handle_input(&data);
		free(data.input);
		free_cmds(&(data.cmds));
	}
	free_mini(&data);
	rl_clear_history();
	return (0);
}
