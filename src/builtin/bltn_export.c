/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:42:22 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/03 14:50:15 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**export_var(char *to_export, char **env)
{
	char *delimiter = ft_strchr(to_export, '=');
	size_t name_length;

    if (delimiter != NULL && delimiter != to_export)
	{
        name_length = delimiter - to_export;

        char *new_var = malloc(name_length + 1 + ft_strlen(delimiter + 1) + 1);
        if (new_var == NULL) {
            perror("malloc failed");
            return (env);
        }
        strncpy(new_var, to_export, name_length);
        new_var[name_length] = '=';
        strcpy(new_var + name_length + 1, delimiter + 1);

        size_t env_count = str_count(env);

        char **new_env = calloc(env_count + 2, sizeof(char*));
        if (new_env == NULL) {
            perror("calloc failed");
            free(new_var);
            return (env);
        }

        for (size_t i = 0; i < env_count; i++) {
            new_env[i] = env[i];
        }

        new_env[env_count] = new_var;
        new_env[env_count + 1] = NULL;

        printf("Exported: %s\n", new_var);
		return (new_env);
    } else {
        printf("Usage: export NAME=VALUE\n");
		return (env);
    }
}

void handle_export(t_mini *data)
{
	char	*to_export;
	char	*input;
	char	**env;
	char	**new_env;

	env = data->env;
	input = data->input + 7;
	while (*input != '\0' && *input != '|')
	{
		if (*input != ' ')
		{
			to_export = strdup_spc(input);
			input += ft_strlen(to_export);
			new_env = export_var(to_export, env);
			if (new_env != env)
			{
				free(env);
				env = new_env;
			}
			free(to_export);
		}
		input++;
	}
	data->env = env;
}