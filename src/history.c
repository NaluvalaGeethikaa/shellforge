#include <stdio.h>
#include <string.h>
#include "history.h"

static char history_list[MAX_HISTORY][256];
static int history_count = 0;

void history_add(const char *cmd) {
    if (!cmd || strlen(cmd) == 0) return;

    if (history_count < MAX_HISTORY) {
        strncpy(history_list[history_count], cmd, 255);
        history_list[history_count][255] = '\0';
        history_count++;
    } else {
        for (int i = 1; i < MAX_HISTORY; i++) {
            strcpy(history_list[i - 1], history_list[i]);
        }
        strncpy(history_list[MAX_HISTORY - 1], cmd, 255);
        history_list[MAX_HISTORY - 1][255] = '\0';
    }
}

void history_print(void) {
    printf("------ Command History ------\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d  %s\n", i + 1, history_list[i]);
    }
    printf("-----------------------------\n");
}
