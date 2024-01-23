/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 07:49:31 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 15:10:09 by svolodin         ###   ########.fr       */
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

// static void rmv_redir(char *redir_start, int left)
// {
// 	char	*redir_end;

// 	redir_end = redir_start;
//     while (*redir_end && !isspace((unsigned char)*redir_end))
//         redir_end++;
//     while (*redir_end && isspace((unsigned char)*redir_end))
// 	{
//         redir_end++;
// 	}
// 	if (!left)
// 	{
// 		while (*redir_end && !isspace((unsigned char)*redir_end))
//         	redir_end++;
// 	}
// 	while (*redir_end)
// 		*redir_start++ = *redir_end++;
//     *redir_start = '\0';
// }

static void rmv_test(char *redir_start)
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
    *redir_start = '\0'; // Null-terminate the modified segment
}



void redirect(char *segment, t_mini *info)
{
    char *redir_symbol;
    char *filename;
    int fd;

    redir_symbol = strstr(segment, "<");
    if (redir_symbol != NULL)
	{
        filename = get_filename(redir_symbol + 1);
		printf(" < filename : %s\n", filename);
        fd = open(filename, O_RDONLY);
        if (fd < 0)
			perror_exit("open");
        info->in_fd = fd;
        //rmv_redir(redir_symbol, 1);
        rmv_test(redir_symbol);
    }

    redir_symbol = strstr(segment, ">");
    if (redir_symbol != NULL) {
        filename = get_filename(redir_symbol + 1);
		printf(" > filename : %s\n", filename);
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
			perror_exit("open");
        info->out_fd = fd;
        // rmv_redir(redir_symbol, 0);
        rmv_test(redir_symbol);
    }
}
