/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:04:50 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 19:29:43 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_echo_flags(char *input, int *newline)
{
	while (*input == ' ')
		input++;
	while (ft_strncmp(input, "-n", 2) == 0)
	{
		input += 2;
		while (*input == 'n')
			input++;
		if (*input == ' ')
			*newline = 0;
		while (*input == ' ')
			input++;
	}
	return (input);
}

char	*get_env_value(char *var, char **env)
{
	char	**splitted;
	char	*value;
	int		i;

	i = -1;
	value = NULL;
	while (env && env[++i] != NULL)
	{
		splitted = ft_split(env[i], '=');
		if (!splitted)
			return (NULL);
		if (strcmp(var, splitted[0]) == 0)
		{
			value = ft_strdup(splitted[1]);
			free_double_array(splitted);
			return (value);
		}
		free_double_array(splitted);
	}
	return (NULL);
}

int	has_even_quotes(const char *input)
{
	int	single_cnt;
	int	double_cnt;

	single_cnt = 0;
	double_cnt = 0;
	while (*input)
	{
		if (*input == '\'')
			single_cnt++;
		if (*input == '"')
			double_cnt++;
		input++;
	}
	if ((single_cnt % 2 == 0) && (double_cnt % 2 == 0))
		return (1);
	else
		return (0);
}

static char	*parse_env_variable_name(char **input_ptr)
{
	char	*input;
	char	*name;

	input = *input_ptr + 1;
	if (*input == '?')
	{
		*input_ptr += 2;
		return (ft_strdup("?"));
	}
	name = strdup_alpha(input);
	if (*name == '\0')
	{
		free(name);
		return (NULL);
	}
	*input_ptr += ft_strlen(name) + 1;
	return (name);
}

char	*expand_env_variable(char **input_ptr, char **env)
{
	char	*name;
	char	*result;
	char	*value;

	if (**input_ptr != '$')
		return (NULL);
	name = parse_env_variable_name(input_ptr);
	if (name == NULL)
		return (NULL);
	result = NULL;
	if (strcmp(name, "?") == 0)
	{
		result = ft_itoa(last_exit_status);
	}
	else
	{
		value = get_env_value(name, env);
		if (value)
			result = ft_strdup(value);
	}
	free(name);
	return (result);
}
