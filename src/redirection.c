/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 07:49:31 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 17:06:01 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

static char *get_filename(char *str)
{
    char	*start;
	char	*end;

    for (start = str; *start && isspace((unsigned char)*start); start++);
    for (end = start; *end && !isspace((unsigned char)*end); end++);

    char *filename = strndup(start, end - start);
    if (filename == NULL)
		perror_exit("strndup");
    return (filename);
}

static void rmv_redir(char *redir_start)
{
    char    *redir_end;
    
    redir_end = redir_start;
	if (*redir_end && (*(redir_end + 1) == ' '))
	{
		redir_end++;
		while (*redir_end && isspace((unsigned char)*redir_end))
    	    redir_end++;
    }
    while (*redir_end && !isspace((unsigned char)*redir_end))
        redir_end++;
    while (*redir_end && isspace((unsigned char)*redir_end))
        redir_end++;
    while (*redir_end)
        *redir_start++ = *redir_end++;
    *redir_start = '\0';
}

//todo	here_doc to add
void	handle_sign(t_mini *info, char *redir_symbol, char *sign)
{
	char	*filename;
	int		fd;

	filename = get_filename(redir_symbol + 1);
	if (ft_strncmp(sign, "<", 2) == 0)
		fd = open(filename, O_RDONLY);
	if (ft_strncmp(sign, ">", 2) == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		printf("%s: No such file or directory\n", filename);
		free(filename);
		return ;
	}
	if (ft_strcmp(sign, "<") == 0)
		info->in_fd = fd;
	if ((ft_strcmp(sign, ">") == 0) || (ft_strcmp(sign, ">>") == 0))
		info->out_fd = fd;
	free(filename);
	rmv_redir(redir_symbol);
}

void redirect(char *segment, t_mini *info)
{
    char *redir_symbol;

    redir_symbol = strstr(segment, "<");
    if (redir_symbol)
		handle_sign(info, redir_symbol, "<");
    redir_symbol = strstr(segment, ">");
    if (redir_symbol)
		handle_sign(info, redir_symbol, ">");
}
