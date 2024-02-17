/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:42:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 11:52:45 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspecial(char symbol)
{
	char	*special_symbols;
	int		i;

	i = 0;
	special_symbols = "!\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~";
	while (special_symbols[i])
	{
		if (symbol == special_symbols[i])
			return (1);
		i++;
	}
	return (0);
}

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
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
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

int	validate_variable_name(char *name)
{
	if (ft_isdigit(name[0]))
		return (-1);
	else if (ft_isspecial(name[0]))
		return (-2);
	return (0);
}

int	process_variable(char **input, char **env)
{
	char	*name;
	char	*value;
	int		result;

	name = NULL;
	value = NULL;
	result = 0;
	if (!find_name(input, &name))
		return (-1);
	if (validate_variable_name(name) == -1)
	{
		free(name);
		return (-1);
	}
	if (*name && !find_value(input, &value, &name))
	{
		free(name);
		return (-1);
	}
	if (name && value)
		result = export_to_env(name, value, env, MAX_ENV_VARS);
	free(value);
	free(name);
	return (result);
}

void	process_input_and_export(char *input, char **env)
{
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input == '\0' || *input == '|')
			break ;
		if (process_variable(&input, env) == -1)
			return ;
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
	if (validate_variable_name(data->cmds[0][1]) == -1)
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(data->cmds[0][1], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	else if (validate_variable_name(data->cmds[0][1]) == -2)
		print_exp_env(data->env);
	else
		process_input_and_export(data->input + 6, data->env);
}
