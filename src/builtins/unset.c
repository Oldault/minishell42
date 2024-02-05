/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:43:09 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/05 13:45:56 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_var(char *to_unset, char **env)
{
	char	*temp;
	int		i;

	i = -1;
	while (env[++i] != NULL)
	{
		temp = (ft_split(env[i], '='))[0];
		if (ft_strcmp(temp, to_unset) == 0)
		{
			free(env[i]);
			free(temp);
			while (env[i] != NULL)
			{
				env[i] = env[i + 1];
				i++;
			}
			break ;
		}
		free(temp);
	}
}

void	handle_unset(t_mini *data)
{
	char	*to_unset;
	char	*input;
	char	**env;
	char	*temp_input;

	env = data->env;
	input = data->input + 5;
	while (*input != '\0' && *input != '|')
	{
		if (*input == ' ')
		{
			input++;
			continue ;
		}
		to_unset = strdup_spc(input);
		temp_input = input;
		while (*temp_input != '\0' && temp_input < input + ft_strlen(to_unset))
			temp_input++;
		input = temp_input;
		unset_var(to_unset, env);
		free(to_unset);
		while (*input == ' ')
			input++;
	}
}
