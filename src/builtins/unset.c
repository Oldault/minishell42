/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:43:09 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/21 12:20:22 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_var(char *to_unset, char **env)
{
	char	**temp;
	char	*temp_name;
	int		i;

	i = -1;
	while (env[++i] != NULL)
	{
		temp = ft_split(env[i], '=');
		temp_name = ft_strdup(temp[0]);
		free_double_array(temp);
		if (ft_strcmp(temp_name, to_unset) == 0)
		{
			free(env[i]);
			free(temp_name);
			while (env[i] != NULL)
			{
				env[i] = env[i + 1];
				i++;
			}
			break ;
		}
		free(temp_name);
	}
}

void	handle_unset(t_mini *data, int cmd_index)
{
	char	*to_unset;
	char	*input;
	char	**env;
	char	*temp_input;

	(void)cmd_index;
	env = data->env;
	input = ft_strstr(data->input + 5, "unset");
	if (!input)
		return ;
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
