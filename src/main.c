/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:06:41 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/19 16:14:08 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_prompt(void)
{
	char	*username;
	char	*hostname;
	char	cwd[1024];
	char	*prompt;

	prompt = malloc(1024 * sizeof(char));
	if (prompt == NULL)
		return (NULL);
	prompt[0] = '\0';
	username = getenv("USER");
	if (username == NULL)
		username = getenv("LOGNAME");
	hostname = "minishell";
	getcwd(cwd, sizeof(cwd));
	ft_strlcat(prompt, COLOR_GREEN, 1024);
	if (username)
		ft_strlcat(prompt, username, 1024);
	ft_strlcat(prompt, "@", 1024);
	ft_strlcat(prompt, hostname, 1024);
	ft_strlcat(prompt, COLOR_BLUE, 1024);
	ft_strlcat(prompt, ":", 1024);
	ft_strlcat(prompt, cwd, 1024);
	ft_strlcat(prompt, "$ ", 1024);
	ft_strlcat(prompt, COLOR_RESET, 1024);
	return (prompt);
}

int	main(void)
{
	char	*input;
	char	*prompt;

	prompt = get_prompt();
	while (42)
	{
		input = readline(prompt);
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		if (strcmp(input, "history") == 0)
			show_hist();
		free(input);
	}
	free(prompt);
	return (0);
}
