/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:52:15 by albeninc          #+#    #+#             */
/*   Updated: 2024/02/03 16:35:20 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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
	REDIR_APPEND,
	REDIR_HEREDOC
}						re_type;

typedef struct
{
	re_type				type;
	char				*filename;
}						redir_t;

typedef struct
{
	redir_t				*redirs;
	int					count;
}						redirs_t;

typedef struct s_mini	t_mini;

typedef struct
{
	char				*command_name;
	void				(*func)(t_mini *);
}						cmd_entry_t;

typedef struct s_mini
{
	char				*input;
	char				*prompt;
	char				**env;
	char				**paths;
	char				***cmds;
	int					in_fd;
	int					out_fd;
	char				*err;
	redirs_t			*redir;
	cmd_entry_t			*bltn;
}						t_mini;

extern int				last_exit_status;

int						handle_builtin(t_mini *data, char *cmd, cmd_entry_t *builtin);

void					ft_signal_fork(int num);
void					ft_signal(int signal);

void					execute_commands(t_mini *data);
char					*get_prompt(void);

int						str_count(char **array);

//*------------------------ Init ------------------------*//
void					set_data_out(t_mini *data, char **env);
void					set_data_in(t_mini *data);

//*---------------------- Builtins ----------------------*//
void					handle_echo(t_mini *data);
void					handle_cd(t_mini *data);
void					handle_pwd(t_mini *data);
void					handle_export(t_mini *data);
void					handle_unset(t_mini *data);
void					handle_env(t_mini *data);
void					handle_exit(t_mini *data);
void					handle_hist(t_mini *data);
void					handle_doll(t_mini *data);

int						path_exists(const char *path);
char					*expand_tilde(const char *input);
char 					*strdup_spc(const char *src);
char 					*strdup_alpha(const char *src);
char 					*get_env_value(char *var, char **env);

//*--------------------- Execution -----------------------*//
void					apply_redirections(t_mini *data, int cmd_index);
void					execute_single_command(t_mini *data, int pipe_end,
							int *pipe_fds, int i, int num_cmds,
							pid_t *child_pids);

//*----------------------- Parse -----------------------*//
int						parse(t_mini *data);
char					**parse_segment(t_mini *data, char *segment, redirs_t *redirections);

//*-------------------- Parse Utils --------------------*//
int						redir_start(char *word);
int						redir_symb(char *word);
re_type					redir_type(char *symbol);
int						get_cmd_len(char **words);
void					redir_split(char *word, redir_t *redirection);

//*----------------------- Paths -----------------------*//
char					**get_paths(t_mini *data, char **env);
char					*find_path(char **paths, char **arg);

//*----------------------- Print -----------------------*//
void					print_2d_arr(char **arr, char separator);
void					print_3d_arr(char ***arr, int clr);
void					print_redir(redir_t **redir_arr);
void					print_redir_blue(t_mini *data);

//*----------------------- Free ------------------------*//
void					free_double_array(char **array);
void					free_triple_array(char ***array);
void					free_cmds(char ****cmds);
void					free_mini(t_mini *data);
void					perror_exit(char *str);
void					free_redir_array(redirs_t *redirections);

#endif
