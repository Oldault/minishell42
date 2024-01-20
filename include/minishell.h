/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:52:15 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/20 15:26:49 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/stat.h>

//		COLORS
# define COLOR_BLUE "\x1B[94m"
# define COLOR_GREEN "\x1B[32m"
# define RED "\x1B[31m"
# define COLOR_RESET "\x1B[0m"

void	show_hist(void);
char	*get_prompt(void);
char	**custom_tokenize(char *input);
char	**tokenizer(const char *str);
void	handle_input(char *input, char **env);
void	setup_signal_handlers(void);

//*----------------------- Print -----------------------*//
void	print_tokens(char **tokens);

#endif