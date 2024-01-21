/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:26:17 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/21 14:38:34 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *input, char **paths, char **env)
{
	char **args = ft_split(input, ' ');
	//print_2d_arr(args, ',');
	
	pid_t pid = fork();
	if (pid == 0)
	{
		if (execve(find_path(paths, args), args, env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
	}
	else
		perror("fork");
}