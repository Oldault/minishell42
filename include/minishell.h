/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:52:15 by albeninc          #+#    #+#             */
/*   Updated: 2024/02/13 11:40:24 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//*--------------------- LIBRARIES ---------------------*//

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>

//*-------------------- DEFINITIONS --------------------*//

# define BLUE "\x1B[94m"
# define GREEN "\x1B[32m"
# define RED "\x1B[31m"
# define COLOR_RESET "\x1B[0m"

# define MAX_ENV_VARS 100

//*---------------------- STRUCTS ----------------------*//

//*                ~~~    Parsing    ~~~                 *//

typedef struct s_parse_seg
{
	char				**args;
	char				*current_arg;
	char				c;
	int					in_single_quote;
	int					in_double_quote;
	int					arg_count;
	int					current_length;
	int					should_expand;
}						t_parse_seg;

//*                ~~~ Redirections  ~~~                 *//
typedef enum s_re_type
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_re_type;

typedef struct s_redir
{
	t_re_type			type;
	char				*filename;
}						t_rdr;

typedef struct s_redirs
{
	t_rdr				*redirs;
	int					count;
}						t_redirs;

//*                ~~~   Builtins   ~~~                 *//
typedef struct s_mini	t_mini;

typedef struct
{
	char				*command_name;
	void				(*func)(t_mini *);
}						t_cmd_entry;

//*                ~~~     Main      ~~~                 *//
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
	t_redirs			*redir;
	t_cmd_entry			*bltn;
	int					seg_count;
}						t_mini;

//*                ~~~  Exit Status  ~~~                 *//

extern int				last_exit_status;

//*------------------------ INIT ------------------------*//
t_mini					*set_data_out(char **env);
void					set_data_in(t_mini *data);
char					*get_prompt(void);

//*---------------------- SIGNALS -----------------------*//
void					ft_signal_fork(int num);
void					ft_signal(int signal);

//*---------------------- BUILTINS ----------------------*//

//*                ~~~  handle each  ~~~                 *//
void					handle_echo(t_mini *data);
void					handle_cd(t_mini *data);
void					handle_pwd(t_mini *data);
void					handle_export(t_mini *data);
void					handle_unset(t_mini *data);
void					handle_env(t_mini *data);
void					handle_exit(t_mini *data);
void					handle_hist(t_mini *data);
void					handle_doll(t_mini *data);

//*                ~~~  export utils  ~~~                *//
int						quote_error(char *input);
void					print_exp_env(char **env);
int						find_name(char **org_input, char **name);
int						find_value(char **org_input, char **value, char **name);

//*                ~~~   echo utils   ~~~                *//
char					*skip_echo_flags(char *input, int *newline);
int						has_even_quotes(const char *input);
char					*expand_env_variable(char **input_ptr, char **env);

int						path_exists(const char *path);
char					*expand_tilde(const char *input);
char					*strdup_spc(const char *src);
char					*strdup_alpha(const char *src);

//*--------------------- Execution -----------------------*//
void					execute_commands(t_mini *data);
void					apply_redirections(t_mini *data, int cmd_index);
void					execute_single_command(t_mini *data, int pipe_end,
							int *pipe_fds, int i, int num_cmds,
							pid_t *child_pids);
int						handle_builtin(t_mini *data, char *cmd,
							t_cmd_entry *builtin);

//*----------------------- Parse -----------------------*//
int						parse(t_mini *data);
char					**parse_segment(t_mini *data, char *segment,
							t_redirs *redirections);
char					**parse_command_segment(char *segment, char **env);
char					*expand_variable(char *var, char **env, int expand);

//*-------------------- Parse Utils --------------------*//
int						redir_start(char *word);
int						redir_symb(char *word);
t_re_type				redir_type(char *symbol);
int						get_cmd_len(char **words);
void					redir_split(char *word, t_rdr *redirection);

//*----------------------- Paths -----------------------*//
char					**get_paths(t_mini *data, char **env);
char					*find_path(char **paths, char **arg);

//*----------------------- Print -----------------------*//
void					print_2d_arr(char **arr, char separator);
void					print_3d_arr(char ***arr, int clr);
void					print_redir(t_rdr **redir_arr);
void					print_redir_blue(t_mini *data);

//*----------------------- Free ------------------------*//
void					free_double_array(char **array);
void					free_triple_array(char ***array);
void					free_cmds(char ****cmds);
void					reset_redirections(t_redirs *redir);
void					reset_data_in(t_mini *data);
void					reset_data_out(t_mini *data);
void					free_seg_cmd_redir(char **seg, char ***cmd,
							t_redirs *redir);
void					perror_exit(char *str);

int						dbl_arr_len(char **arr);

#endif
