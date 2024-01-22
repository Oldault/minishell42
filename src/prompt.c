/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:16:23 by svolodin          #+#    #+#             */
/*   Updated: 2024/01/19 17:29:57 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROMPT_SIZE 1024
#define PATH_SIZE 1024

// Cette fonction raccourcit le chemin en remplaçant le répertoire personnel par '~'
void shorten_path(char *path, const char *home_dir) {
    if (home_dir && strncmp(path, home_dir, strlen(home_dir)) == 0) {
        memmove(path, path + strlen(home_dir), strlen(path) - strlen(home_dir) + 1);
        path[0] = '~';  // Remplacer le début par '~'
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

    // Construction du prompt
    strncat(prompt, "[", PROMPT_SIZE - strlen(prompt) - 1);
    strncat(prompt, username, PROMPT_SIZE - strlen(prompt) - 1);
    strncat(prompt, "@minishell ", PROMPT_SIZE - strlen(prompt) - 1);
    strncat(prompt, cwd, PROMPT_SIZE - strlen(prompt) - 1);
    strncat(prompt, "]$ ", PROMPT_SIZE - strlen(prompt) - 1);

    return prompt;
}
