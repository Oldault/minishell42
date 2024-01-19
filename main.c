#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void afficher_historique() {
    HIST_ENTRY **liste_historique = history_list();
    if (liste_historique) {
        for (int i = 0; liste_historique[i]; ++i) {
            printf("%d: %s\n", i + 1, liste_historique[i]->line);
        }
    }
}

int main() {
    char *input;
    while ((input = readline("Minishell :> ")) != NULL) {
        if (*input) {
            add_history(input);
        }

        if (strcmp(input, "history") == 0) {
            afficher_historique();
        }

        free(input);
    }
    return 0;
}
