/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:52:15 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/21 14:30:57 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/stat.h>
# include <sys/wait.h>

//		COLORS
# define COLOR_BLUE "\x1B[94m"
# define COLOR_GREEN "\x1B[32m"
# define RED "\x1B[31m"
# define COLOR_RESET "\x1B[0m"

int	g_status;

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	pid_t	pid;
}		t_prompt;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	int	infile;
	int	outfile;
}		t_mini;

void	show_hist(void);
char	*get_prompt(void);
char	**tokenizer(const char *str);
void	handle_input(char *input, char **env, char **paths);
void	setup_signal_handlers(void);
void	execute_command(char *input, char **paths, char **env);

//*----------------------- Paths -----------------------*//
char	**get_paths(char **env);
char	*find_path(char **paths, char **arg);

//*----------------------- Print -----------------------*//
void	print_tokens(char **tokens);
void	print_2d_arr(char **arr, char separator);
void	print_3d_arr(char ***arr);

#endif