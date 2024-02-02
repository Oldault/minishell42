/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:39:43 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/02 16:40:03 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_echo_flags(char *input, int *newline)
{
	while (*input == ' ')
		input++;
	while (ft_strncmp(input, "-n", 2) == 0)
	{
		*newline = 0;
		input += 2;
		while (*input == 'n')
			input++;
		while (*input == ' ')
			input++;
	}
	return (input);
}


static char *get_env_value(char *var, char **env)
{
	char **splitted;
	
    for (int i = 0; env && env[i] != NULL; i++)
	{
		splitted = ft_split(env[i], '=');
        if (!splitted)
			return (NULL);
        if (strcmp(var, splitted[0]) == 0)
		{
            return (splitted[1]);
        }
        free_double_array(splitted);
    }
    return (NULL);
}

void handle_echo(t_mini *data)
{
	int		newline;
	char	*input;
	char	*name;
	char	*value;

	newline = 1;
	input = skip_echo_flags(data->input + 5, &newline);
	while (*input != '\0' && *input != '|')
	{
		if (*input == '$')
		{
			if (*(input + 1) == '?')
				printf("%d", last_exit_status);
			name = strdup_spc(input + 1);
			value = get_env_value(name, data->env);
			if (value)
				printf("%s", value);
			input += ft_strlen(name) + 1;
			free(name);
		}
		else
			printf("%c", *input);
		input++;
	}
	
    printf("%s", input);
	if (newline)
		printf("\n");
}