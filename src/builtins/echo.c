/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:39:43 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 12:16:36 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_single_quotes(char **input_ptr)
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

static void	handle_expand(char **input, char **env)
{
	char	*expanded;

	expanded = expand_env_variable(input, env);
	if (expanded)
	{
		printf("%s", expanded);
		free(expanded);
	}
	else
	{
		printf("$");
		if (*(input + 1))
			*input += 1;
	}
}

static char	*handle_double_quotes(char **input_ptr, char **env)
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

static char	*handle_unquoted_text(char **input_ptr, char **env)
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

void	handle_echo(t_mini *data, int cmd_index)
{
	int		newline;
	char	*input;

	(void)cmd_index;
	newline = 1;
	input = skip_echo_flags(ft_strstr(data->input, "echo") + 5, &newline);
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
