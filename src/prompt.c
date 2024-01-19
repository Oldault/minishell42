/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:16:23 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/19 16:18:18 by svolodin         ###   ########.fr       */
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
	ft_strlcat(prompt, ":", 1024);
	ft_strlcat(prompt, COLOR_BLUE, 1024);
	ft_strlcat(prompt, cwd, 1024);
	ft_strlcat(prompt, "$ ", 1024);
	ft_strlcat(prompt, COLOR_RESET, 1024);
	return (prompt);
}
