/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:39:43 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 15:23:19 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_echo_flags(char *input, int *newline)
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

char	*expand_env_variable(char **input_ptr, char **env)
{
	char	*name;
	char	*value;
	char	*result;
	char	*input;

	input = *input_ptr;
	if (*input == '$')
	{
		if (*(input + 1) == '\0')
			return (*input_ptr += 1, NULL);
		input++;
		if (*input == '?')
			return (*input_ptr += 2, ft_itoa(last_exit_status));
		name = strdup_alpha(input);
		if (*name == '\0')
			return (free(name), *input_ptr += 1, NULL);
		value = get_env_value(name, env);
		if (value)
		{
			result = ft_strdup(value);
			free(value);
		}
		else
			result = NULL;
		*input_ptr += (ft_strlen(name) + 1);
		free(name);
		return (result);
	}
	return (NULL);
}

char	*handle_single_quotes(char **input_ptr)
{
	char	*input;

	input = *input_ptr + 1;
	while (*input && *input != '\'')
		printf("%c", *input++);
	if (*input == '\'')
		input++;
	*input_ptr = input;
	return (input);
}

void	handle_expand(char **input, char **env)
{
	char	*expanded;

	expanded = expand_env_variable(input, env);
	if (expanded)
	{
		printf("%s", expanded);
		free(expanded);
	}
	else
		printf("$");
}

char	*handle_double_quotes(char **input_ptr, char **env)
{
	char	*input;

	input = *input_ptr + 1;
	while (*input && *input != '"')
	{
		if (*input == '$')
			handle_expand(&input, env);
		else
			printf("%c", *input++);
	}
	if (*input == '"')
		input++;
	*input_ptr = input;
	return (input);
}

char	*handle_unquoted_text(char **input_ptr, char **env)
{
	char	*input;

	input = *input_ptr;
	while (*input && *input != '"' && *input != '\'' && *input != '|'
		&& *input != '\0')
	{
		if (*input == '$')
			handle_expand(&input, env);
		else
			printf("%c", *input++);
	}
	*input_ptr = input;
	return (input);
}

void	handle_echo(t_mini *data)
{
	int		newline;
	char	*input;

	newline = 1;
	input = skip_echo_flags(data->input + 5, &newline);
	if (!has_even_quotes(input))
	{
        printf("Error: Mismatched quotes in input\n");
        return ;
    }
	while (*input != '\0' && *input != '|')
	{
		if (*input == '\'')
			handle_single_quotes(&input);
		else if (*input == '"')
			handle_double_quotes(&input, data->env);
		else
			handle_unquoted_text(&input, data->env);
	}
	if (newline)
		printf("\n");
}
