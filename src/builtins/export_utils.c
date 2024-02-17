/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:01:15 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/15 12:42:01 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exp_env(char **env)
{
	int	i;
	int	j;

	i = -1;
	while (env[++i])
	{
		j = -1;
		printf("declare -x ");
		while (env[i][++j])
		{
			printf("%c", env[i][j]);
			if (env[i][j] && env[i][j] == '=')
				printf("\"");
			if (!(env[i][j + 1]))
				printf("\"\n");
		}
	}
}

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

char	*get_env_value(char *var_name, char **env)
{
	int		i;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(var_name);
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, name_len) == 0
			&& env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

int	find_name(char **org_input, char **name)
{
	int		i;
	char	*input;

	i = 0;
	input = *org_input;
	while (input[i])
	{
		if (input[i] == '=')
		{
			*name = ft_strndup(input, i);
			if (*name == NULL)
				return (0);
			*org_input += ft_strlen(*name) + 1;
			return (1);
		}
		i++;
	}
	printf("Error: Problem getting name\n");
	return (0);
}

int	find_value(char **org_input, char **value, char **name)
{
	int		i;
	char	*in;

	i = 0;
	in = *org_input;
	if (in[i] == '\"')
	{
		in++;
		while (in[i] && in[i] != '\"')
			i++;
		if (quote_error(in))
			return (printf("Error: Problem getting value\n"), free(*name), 0);
		*value = ft_strndup(in, i);
		*org_input += i + 2;
	}
	else
	{
		while (in[i] && in[i] != ' ' && in[i] != '\0' && in[i] != '|')
			i++;
		*value = ft_strndup(in, i);
		*org_input += i;
	}
	if (!*value)
		return (printf("Error: Problem allocating value\n"), free(*name), 0);
	return (1);
}
