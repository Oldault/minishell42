/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:16:23 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/23 15:11:33 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PROMPT_SIZE 1024
#define PATH_SIZE 1024

void shorten_path(char *path, const char *home_dir) {
    if (home_dir && strncmp(path, home_dir, strlen(home_dir)) == 0) {
        size_t home_dir_length = strlen(home_dir);
        size_t path_length = strlen(path);

        // If the path is exactly the home directory or the home directory plus a slash, replace with "~"
        if (home_dir_length == path_length || (home_dir_length + 1 == path_length && path[home_dir_length] == '/')) {
            strcpy(path, "~");
        } else {
            // Replace the home directory part with "~/" and shift the rest of the path
            // The +1 in the memmove start position skips the slash after the home directory
            memmove(path + 2, path + home_dir_length + 1, path_length - home_dir_length);
            path[0] = '~';
            path[1] = '/';
            path[path_length - home_dir_length + 1] = '\0'; // Properly terminate the string
        }
    }
}


char *get_prompt(void) {
    char *prompt = malloc(PROMPT_SIZE);
    if (prompt == NULL) {
        perror("Unable to allocate memory for prompt");
        return NULL;
    }

    char cwd[PATH_SIZE];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Unable to get current working directory");
        free(prompt);
        return NULL;
    }

    const char *home_dir = getenv("HOME");
    const char *username = getenv("USER");
    if (username == NULL) {
        username = "user";  // Nom d'utilisateur par défaut si USER n'est pas défini
    }

    shorten_path(cwd, home_dir);

    // Réinitialiser le contenu de prompt
    prompt[0] = '\0';
    strncat(prompt, username, PROMPT_SIZE - strlen(prompt) - 1);
    strncat(prompt, "@minishell:", PROMPT_SIZE - strlen(prompt) - 1);
    strncat(prompt, cwd, PROMPT_SIZE - strlen(prompt) - 1);
    strncat(prompt, "$ ", PROMPT_SIZE - strlen(prompt) - 1);
    return (prompt);
}
