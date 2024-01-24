/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 11:49:03 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/22 20:39:08 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int	last_exit_status = 0;
int last_command_was_dollar = 0;

void update_exit_status(int status) {
    last_exit_status = status;
}

void execute_command(char *cmd) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        last_exit_status = 1;  // Set exit status to 1 on fork failure
    } else if (pid == 0) {
        // Child process: Execute the command
        if (execlp(cmd, cmd, (char *)NULL) == -1) {
            fprintf(stderr, "%s: command not found\n", cmd);
            exit(127);  // Exit with status 127 if command not found
        }
    } else {
        // Parent process: Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGUSR1) {
            last_exit_status = 127;  // Set exit status to 127 if command not found
        }
    }
}


void int_to_str(int num, char *str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    int i = 0, isNegative = 0;

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void expand_exit_status(char *cmd, char *expanded_cmd) {
    char exit_status_str[20];  // Buffer for the exit status as a string

    // Convert last_exit_status to a string
    int_to_str(last_exit_status, exit_status_str);

    // Search for $? in cmd
    char *dollar_pos = strstr(cmd, "$?");
    if (dollar_pos != NULL) {
        ptrdiff_t chars_before_dollar = dollar_pos - cmd;
        strncpy(expanded_cmd, cmd, chars_before_dollar);
        expanded_cmd[chars_before_dollar] = '\0';

        // Concatenate the exit status and the rest of cmd
        strcat(expanded_cmd, exit_status_str);
        strcat(expanded_cmd, dollar_pos + 2);
    } else {
        strcpy(expanded_cmd, cmd);
    }
}

void	handle_sigint(int sig)
{
	(void)sig;
	printf("%s", get_prompt());
}

//	! Ctrl+D Not working
void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

int    do_signal(char *input, int *last_command_was_dollar, int *last_exit_status)
{
    if (strcmp(input, "$?") == 0) {
        if (*last_command_was_dollar) {
            *last_exit_status = 127;
        }
        printf("%d : command not found\n", *last_exit_status);
        *last_command_was_dollar = 1;
        free(input);
        return 1;
    } else {
        *last_command_was_dollar = 0;
        return 0;
    }
}
