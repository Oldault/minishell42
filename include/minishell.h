/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:52:15 by albeninc          #+#    #+#             */
/*   Updated: 2024/02/20 12:09:07 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//*--------------------- 📚 𝙇𝙄𝘽𝙍𝘼𝙍𝙄𝙀𝙎 📚------------------------*//

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>

//*-------------------- 📖 𝘿𝙀𝙁𝙄𝙉𝙄𝙏𝙄𝙊𝙉𝙎 📖 ---------------------*//

# define BLUE "\x1B[94m"
# define GREEN "\x1B[32m"
# define RED "\x1B[31m"
# define COLOR_RESET "\x1B[0m"

# define MAX_ENV_VARS 100

//*---------------------- ⚙️ 𝙎𝙏𝙍𝙐𝘾𝙏𝙎 ⚙️ -----------------------*//

//?                   ~~~    𝙿𝚊𝚛𝚜𝚒𝚗𝚐    ~~~                    *//
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

//?                   ~~~   𝙴𝚡𝚎𝚌𝚞𝚝𝚒𝚘𝚗   ~~~                   *//
typedef struct s_exec_cmd
{
	int					num_cmds;
	int					status;
	int					pipe_fds[2];
	int					pipe_end;
	pid_t				*child_pids;
	char				*cmd_path;
}						t_exec_cmd;

//?                   ~~~  𝚁𝚎𝚍𝚒𝚛𝚎𝚌𝚝𝚒𝚘𝚗𝚜 ~~~                   *//
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

//?                   ~~~   𝙱𝚞𝚒𝚕𝚝𝚒𝚗𝚜   ~~~                    *//
typedef struct s_mini	t_mini;

typedef struct s_cmd_entry
{
	char				*command_name;
	void				(*func)(t_mini *);
}						t_cmd_entry;

//?                   ~~~     𝙼𝚊𝚒𝚗      ~~~                   *//
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

//?                   ~~~  𝙴𝚡𝚒𝚝 𝚂𝚝𝚊𝚝𝚞𝚜  ~~~                  *//
extern int				g_exit_stat;

//*------------------------ 🚀 𝙄𝙉𝙄𝙏 🚀 -----------------------*//
t_mini					*set_data_out(char **env);
void					set_data_in(t_mini *data);
char					*get_prompt(char **env);
int						dbl_arr_len(char **arr);

//*---------------------- 📡 𝙎𝙄𝙂𝙉𝘼𝙇𝙎 📡 ----------------------*//
void					ft_signal(int signal);
void					ft_signal_fork(int num);
void					ft_signal_heredoc(int signal);

//*----------------------- 🚧 𝙋𝘼𝙏𝙃𝙎 🚧 -----------------------*//
char					**get_paths(t_mini *data, char **env);
char					*find_path(char **paths, char **arg);
char					*ft_getenv(char **env, char *name);

//*----------------------- 👁️ 𝙋𝘼𝙍𝙎𝙀 👁️ -----------------------*//
int						parse(t_mini *data);
char					**parse_segment(t_mini *data, char *segment,
							t_redirs *redirections);
char					**parse_command_segment(char *segment, char **env);
char					*expand_variable(char *var, char **env, int expand);

// todo               ~~~   𝚙𝚊𝚛𝚜𝚎 𝚞𝚝𝚒𝚕𝚜   ~~~                *//
int						redir_start(char *word);
int						redir_symb(char *word);
t_re_type				redir_type(char *symbol);
int						get_cmd_len(char **words);
void					redir_split(char *word, t_rdr *redirection);
int						invalid_pipes(char **segments, int seg_num,
							t_mini *data);
int						echo_pipe(t_mini *data);
int						is_only_spaces(const char *str);

// todo               ~~~   𝚑𝚊𝚗𝚍𝚕𝚎 𝚟𝚊𝚛    ~~~                *//
int						handle_single_quote(t_parse_seg *pdata);
int						handle_double_quote(t_parse_seg *pdata);
int						handle_space(t_parse_seg *pdata);
void					handle_nonexistent_variable(t_parse_seg *pdata,
							char *var_name, size_t var_len);

// todo               ~~~  𝚑𝚊𝚗𝚍𝚕𝚎 𝚎𝚡𝚙𝚊𝚗𝚍  ~~~                *//
int						handle_expansion(char *segment, size_t *i,
							t_parse_seg *pdata, char **env);

//*---------------------- 🌍 𝘽𝙐𝙄𝙇𝙏𝙄𝙉𝙎 🌍 ----------------------*//
// todo               ~~~   𝚑𝚊𝚗𝚍𝚕𝚎 𝚎𝚊𝚌𝚑   ~~~                 *//
void					handle_echo(t_mini *data);
void					handle_cd(t_mini *data);
void					handle_pwd(t_mini *data);
void					handle_export(t_mini *data);
void					handle_unset(t_mini *data);
void					handle_env(t_mini *data);
void					handle_exit(t_mini *data, char **args);
void					handle_hist(t_mini *data);
void					handle_doll(t_mini *data);

// todo               ~~~  𝚎𝚡𝚙𝚘𝚛𝚝 𝚞𝚝𝚒𝚕𝚜  ~~~                  *//
int						quote_error(char *input);
int						validate_variable_name(char *name);
void					print_exp_env(char **env);
int						find_name(char **org_input, char **name);
int						find_value(char **org_input, char **value, char **name);
char					*get_env_value(char *var_name, char **env);
int						handle_value_assignment(char *name, char **value,
							char **env);
int						export_to_env(char *name, char *value, char **env,
							int max_env_size);

// todo               ~~~   𝚎𝚌𝚑𝚘 𝚞𝚝𝚒𝚕𝚜   ~~~                  *//
char					*skip_echo_flags(char *input, int *newline);
int						has_even_quotes(const char *input);
char					*expand_env_variable(char **input_ptr, char **env);

int						path_exists(const char *path);
char					*strdup_spc(const char *src);
char					*strdup_alpha(const char *src);
char					*handle_dollar_only(char *name);
char					*handle_exit_status(char *name);

//*--------------------- 🖥️ 𝙀𝙓𝙀𝘾𝙐𝙏𝙄𝙊𝙉 🖥️ ---------------------*//
void					execute_commands(t_mini *data);
int						is_builtin(char *cmd, t_mini *data);
t_exec_cmd				*init_exec_data(t_mini *data);
int						handle_builtin(t_mini *data);
int						handle_cmd_path(char *cmd, char *cmd_path,
							t_exec_cmd *exec_data);
void					setup_pipes(int *pipe_fds, int i, int num_cmds);
void					apply_redirections(t_mini *data, int cmd_index);
void					execute_single_command(t_mini *data,
							t_exec_cmd *exec_data, int i);

//*----------------------- 🖨️ 𝙋𝙍𝙄𝙉𝙏 🖨️ -----------------------*//
void					print_2d_arr(char **arr, char separator);
void					print_3d_arr(char ***arr, int clr);
void					print_redir(t_rdr **redir_arr);
void					print_redir_blue(t_mini *data);

//*----------------------- 🌬️ 𝙁𝙍𝙀𝙀 🌬️ ------------------------*//
void					free_double_array(char **array);
void					free_triple_array(char ***array);
void					free_cmds(char ****cmds);
void					reset_redirections(t_redirs *redir);
void					reset_data_mid(t_mini *data);
void					reset_data_in(t_mini *data);
void					reset_data_out(t_mini *data);
void					free_seg_cmd_redir(char **seg, char ***cmd,
							t_redirs *redir);
void					perror_exit(char *str);

#endif
