/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_seg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:19:48 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/04 18:37:22 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variable(char *var, char **env, int expand)
{
	if (!expand)
		return (strdup(var));
	for (int i = 0; env && env[i]; i++)
	{
		if (strncmp(env[i], var, strlen(var)) == 0
			&& env[i][strlen(var)] == '=')
		{
			return (strdup(env[i] + strlen(var) + 1));
		}
	}
	return (NULL);
}

char	**parse_command_segment(char *segment, char **env)
{
	char	**args;
	char	*current_arg;
	char	*expanded_value;
	char	c;
	int		var_len = 0;
	int		in_single_quote = 0;
	int		in_double_quote = 0;
	int		arg_count = 0;
	int		current_length = 0;
	int		should_expand = 1;

	args = malloc(sizeof(char *) * (strlen(segment) / 2 + 2));
	current_arg = malloc(strlen(segment) + 1);
	for (int i = 0; segment[i]; ++i)
	{
		c = segment[i];
		if (c == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			should_expand = !in_single_quote;
			continue ;
		}
		else if (c == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			continue ;
		}
		else if (c == ' ' && !in_single_quote && !in_double_quote)
		{
			if (current_length > 0)
			{
				current_arg[current_length] = '\0';
				args[arg_count++] = strdup(current_arg);
				current_length = 0;
			}
			continue ;
		}
		else if (c == '$' && should_expand)
		{
			char var_name[256];
			i++;
			while (segment[i] && (ft_isalnum(segment[i]) || segment[i] == '_'))
			{
				var_name[var_len++] = segment[i++];
			}
			var_name[var_len] = '\0';
			expanded_value = expand_variable(var_name, env, should_expand);
			strcpy(current_arg + current_length, expanded_value);
			current_length += strlen(expanded_value);
			free(expanded_value);
			i--;
			continue ;
		}
		current_arg[current_length++] = c;
	}
	if (current_length > 0)
	{
		current_arg[current_length] = '\0';
		args[arg_count++] = strdup(current_arg);
	}
	free(current_arg);
	args[arg_count] = NULL;
	return (args);
}
