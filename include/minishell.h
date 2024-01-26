/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:52:15 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/26 16:23:20 by svolodin         ###   ########.fr       */
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
# define BLUE "\x1B[94m"
# define GREEN "\x1B[32m"
# define RED "\x1B[31m"
# define COLOR_RESET "\x1B[0m"

typedef enum
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND
}				re_type;

typedef struct
{
	re_type		type;
	char		*filename;
}				redir_t;

typedef struct
{
	redir_t		*redirs;
	int			count;
}				redirs_t;

typedef struct s_mini
{
	char		*input;
	char		*prompt;
	char		**env;
	char		**paths;
	char		***cmds;
	int			in_fd;
	int			out_fd;
	redirs_t	*redir;
}				t_mini;

void			show_hist(void);
void			handle_input(t_mini *info);
void			setup_signal_handlers(void);
int				do_signal(char *input, int *lst_cmd_dlr, int *lst_ext_stat);
void			execute_commands(t_mini *info);
char			*get_prompt(void);
void			redirect(char *segment, t_mini *info);

//*----------------------- Parse -----------------------*//
int				parse(t_mini *info);
char			**parse_segment(char *segment, redirs_t *redirections);

//*-------------------- Parse Utils --------------------*//
int				redir_start(char *word);
int				redir_symb(char *word);
re_type			redir_type(char *symbol);
int				get_cmd_len(char **words);
void			redir_split(char *word, redir_t *redirection);

//*----------------------- Paths -----------------------*//
char			**get_paths(char **env);
char			*find_path(char **paths, char **arg);

//*----------------------- Print -----------------------*//
void			print_2d_arr(char **arr, char separator);
void			print_3d_arr(char ***arr, int clr);
void			print_redir(redir_t **redir_arr);
void			print_redir_blue(t_mini *info);

//*----------------------- Free ------------------------*//
void			free_double_array(char **array);
void			free_triple_array(char ***array);
void			free_cmds(char ****cmds);
void			free_mini(t_mini *info);
void			perror_exit(char *str);
void			free_redir_array(redirs_t *redirections);

#endif
