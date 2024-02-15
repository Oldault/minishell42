/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:42:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 11:30:50 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_to_env(char *name, char *value, char **env, int max_env_size)
{
	size_t	name_len;
	char	*new_var;
	int		i;

	name_len = ft_strlen(name);
	new_var = malloc(name_len + ft_strlen(value) + 2);
	if (new_var == NULL)
		return (ft_putstr_fd("Error creating new env var\n", 2), -1);
	ft_strcpy(new_var, name);
	strcat(new_var, "=");
	strcat(new_var, value);
	i = -1;
	while (++i < max_env_size)
	{
		if (env[i] == NULL)
			return (env[i] = new_var, -1);
		else if (ft_strncmp(env[i], name, name_len) == 0
			&& env[i][name_len] == '=')
		{
			return (free(env[i]), env[i] = new_var, 1);
		}
	}
	return (ft_putstr_fd("Error : env is full\n", 2), free(new_var), 0);
}

void	process_input_and_export(char *input, char **env)
{
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input == '\0' || *input == '|')
			break ;
		if (!find_name(&input, &name))
			return ;
		if (*input && !find_value(&input, &value, &name))
			return ;
		if (name && value)
			export_to_env(name, value, env, MAX_ENV_VARS);
		if (value)
			free(value);
		free(name);
		while (*input == ' ')
			input++;
	}
}

void	handle_export(t_mini *data)
{
	if (data->cmds[0][1] == NULL)
	{
		print_exp_env(data->env);
		return ;
	}
	process_input_and_export(data->input + 6, data->env);
}
