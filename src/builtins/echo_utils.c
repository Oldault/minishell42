/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:04:50 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/13 11:42:51 by svolodin         ###   ########.fr       */
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

char	*handle_dollar_only(char *name)
{
	free(name);
	return (ft_strdup("$"));
}

char	*handle_exit_status(char *name)
{
	free(name);
	return (ft_itoa(last_exit_status));
}

char	*handle_non_existent_variable(char *name, char *input_ptr, char **env)
{
	char	*value;

	value = expand_variable(name, env, 1);
	if (value)
	{
		free(name);
		return (value);
	}
	else if (*input_ptr == '$' && !value)
	{
		free(name);
		return (ft_strdup("$"));
	}
	else
	{
		free(name);
		return (ft_strdup(""));
	}
}

char	*expand_env_variable(char **input_ptr, char **env)
{
	char	*name;

	if (**input_ptr != '$')
		return (NULL);
	name = parse_env_variable_name(input_ptr);
	if (!name)
		return (NULL);
	if (ft_strcmp(name, "") == 0)
		return (handle_dollar_only(name));
	else if (ft_strcmp(name, "?") == 0)
		return (handle_exit_status(name));
	else
		return (handle_non_existent_variable(name, *input_ptr, env));
	return (NULL);
}
