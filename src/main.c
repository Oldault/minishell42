/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/19 17:55:37 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_env(char **env)
{
	int	i;

	i = -1;
    while (env[++i] != NULL)
        printf("%s\n", env[i]);
}

void	handle_input(char *input, char **env)
{
	if (ft_strncmp(input, "history", 8) == 0)
			show_hist();
	else if (ft_strncmp(input, "exit", 5) == 0)
	{
		free(input);
		exit(0);
	}
	else if (ft_strncmp(input, "env", 4) == 0)
		execute_env(env);
}

int	main(int ac, char **av, char ** env)
{
	char	*input;
	char	*prompt;
    char	**tokens;

	(void)ac;
	(void)av;
	prompt = get_prompt();
	while (42)
	{
		input = readline(prompt);
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		handle_input(input, env);
		tokens = custom_tokenize(input);

        for (int i = 0; tokens[i] != NULL; i++) {
            printf("Token %d: %s\n", i, tokens[i]);
            free(tokens[i]);
        }
        free(tokens);
		free(input);
	}
	free(prompt);
	return (0);
}
