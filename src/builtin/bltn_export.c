/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:42:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/04 12:28:25 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_error(char *input)
{
	while (*input)
	{
		if (*input == '\"')
			return (0);
		input++;
	}
	return (1);
}

char	*find_name(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '=')
			return (strndup(input, i));
		i++;
	}
	return (NULL);
}

char	*find_value(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '\"')
	{
		input++;
		if (quote_error(input))
			return (printf("Quote error\n"), NULL);
		while (input[i] && input[i] != '\"')
			i++;
		return (strndup(input, i));
	}
	while (input[i] && input[i] != ' ' && input[i] != '\0' && input[i] != '|')
		i++;
	return (strndup(input, i));
}

void	export_to_env(char *name, char *value, char **env, int max_env_size)
{
	size_t	name_len;
	char	*new_var;

	name_len = ft_strlen(name);
	new_var = malloc(name_len + ft_strlen(value) + 2);
	if (new_var == NULL)
	{
		ft_putstr_fd("Error creating new env var\n", 2);
		return ;
	}
	strcpy(new_var, name);
	strcat(new_var, "=");
	strcat(new_var, value);
	for (int i = 0; i < max_env_size; i++)
	{
		if (env[i] == NULL)
		{
			env[i] = new_var;
			return ;
		}
		else if (strncmp(env[i], name, name_len) == 0
			&& env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return ;
		}
	}
	ft_putstr_fd("Error : env is full\n", 2);
	free(new_var);
}

void	handle_export(t_mini *data)
{
	char	*input;
	char	**env;
	char	*name;
	char	*value;

	env = data->env;
	input = data->input + 7;
	name = NULL;
	value = NULL;
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input == '\0' || *input == '|')
			break ;
		name = find_name(input);
		if (!name)
		{
			printf("Error: Problem getting name\n");
			return ;
		}
		input += ft_strlen(name) + 1;
		if (*input)
		{
			value = find_value(input);
			if (!value)
			{
				printf("Error: Problem getting value\n");
				free(name);
				return ;
			}
			if (*input == '\"')
				input += strlen(value) + 2;
			else
				input += strlen(value);
		}
		if (name && value)
			export_to_env(name, value, data->env, MAX_ENV_VARS);
		if (value)
			free(value);
		free(name);
		while (*input == ' ')
			input++;
	}
}
