/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_linked_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albeninc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 16:15:36 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/21 16:54:26 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

typedef struct s_mini {
    char **full_cmd;
} t_mini;

t_mini *new_mini_node(char **cmd) {
    t_mini *node = malloc(sizeof(t_mini));
    if (node == NULL) {
        perror("Failed to allocate t_mini");
        return NULL;
    }
    node->full_cmd = cmd;
    return node;
}

t_list *add_to_list(t_list *head, t_mini *cmd) {
    t_list *new_node = malloc(sizeof(t_list));
    if (new_node == NULL) {
        perror("Failed to allocate t_list node");
        return head;
    }
    new_node->content = cmd;
    new_node->next = NULL;

    if (head == NULL) {
        return new_node;
    }

    t_list *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return head;
}

t_list *create_command_list(char **commands) {
    t_list *cmd_list = NULL;
    char **cmd_segment = NULL;
    int cmd_index = 0;
    int segment_size = 100;  // Starting size of each command segment

    for (int i = 0; commands[i] != NULL; ++i) {
        if (cmd_segment == NULL) {
            cmd_segment = malloc(sizeof(char *) * segment_size);
            cmd_index = 0;
        }

        if (strcmp(commands[i], "|") == 0) {
            // End the current command segment and add to the list
            cmd_segment[cmd_index] = NULL;
            cmd_list = add_to_list(cmd_list, new_mini_node(cmd_segment));
            cmd_segment = NULL;  // Reset cmd_segment for the next command
        } else {
            // Add the command/argument to the current segment
            cmd_segment[cmd_index++] = strdup(commands[i]);

            // Expand cmd_segment if necessary
            if (cmd_index >= segment_size - 1) {
                segment_size *= 2;
                cmd_segment = realloc(cmd_segment, sizeof(char *) * segment_size);
            }
        }
    }

    // Add the last command segment if it exists
    if (cmd_segment != NULL) {
        cmd_segment[cmd_index] = NULL;
        cmd_list = add_to_list(cmd_list, new_mini_node(cmd_segment));
    }

    return cmd_list;
}

void free_command_list(t_list *list) {
    while (list != NULL) {
        t_list *temp = list;
        list = list->next;

        t_mini *cmd = (t_mini *)(temp->content);  // Cast to t_mini *
        if (cmd) {
            for (int i = 0; cmd->full_cmd && cmd->full_cmd[i]; ++i) {
                free(cmd->full_cmd[i]);  // Free each string
            }
            free(cmd->full_cmd);  // Free the array
            free(cmd);            // Free the t_mini structure
        }

        free(temp);  // Free the list node
    }
}

int main() {
    // Simulated input string
    const char *input_str = "ls -l | cat | echo hello";

    // Tokenize the input string
    char **tokens = tokenizer(input_str);
    if (!tokens) {
        perror("Tokenization failed");
        return EXIT_FAILURE;
    }

    // Create a command list from the tokens
    t_list *cmd_list = create_command_list(tokens);

    // Iterate over the list and print each command
    for (t_list *current = cmd_list; current != NULL; current = current->next) {
        t_mini *cmd = (t_mini *)(current->content);
        printf("Command: ");
        for (int i = 0; cmd && cmd->full_cmd[i] != NULL; ++i) {
            printf("'%s' ", cmd->full_cmd[i]);
        }
        printf("\n");
    }

    // Free the command list and the tokens
    free_command_list(cmd_list);
    for (int i = 0; tokens[i] != NULL; ++i) {
        free(tokens[i]);
    }
    free(tokens);

    return 0;
}
