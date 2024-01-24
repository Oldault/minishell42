/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:52:15 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/23 08:04:42 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/stat.h>
# include <sys/wait.h>

//		COLORS
# define COLOR_BLUE "\x1B[94m"
# define COLOR_GREEN "\x1B[32m"
# define RED "\x1B[31m"
# define COLOR_RESET "\x1B[0m"

typedef struct s_mini
{
	char	*input;
	char	*prompt;
	char	**env;
	char	**paths;
	char	***cmds;
	int		in_fd;
	int		out_fd;
}			t_mini;

void		show_hist(void);
void		handle_input(t_mini *info);
void		setup_signal_handlers(void);
int     do_signal(char *input, int *last_command_was_dollar, int *last_exit_status);
void    execute_commands(t_mini *info);
char		*get_prompt(void);
void		redirect(char *segment, t_mini *info);


//*----------------------- Parse -----------------------*//
char		***parse(t_mini *info);

//*----------------------- Paths -----------------------*//
char		**get_paths(char **env);
char		*find_path(char **paths, char **arg);

//*----------------------- Print -----------------------*//
void		print_tokens(char **tokens);
void		print_2d_arr(char **arr, char separator);
void		print_3d_arr(char ***arr);

//*----------------------- Free ------------------------*//
void		free_double_array(char **array);
void		free_triple_array(char ***array);
void		free_cmds(char ****cmds);
void		free_mini(t_mini *info);
void		perror_exit(char *str);

#endif
