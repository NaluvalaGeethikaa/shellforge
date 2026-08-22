#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "builtin.h"

int builtin_pwd(char **args)
{
    char cwd[PATH_MAX];

    if (args[1] != NULL) {
        printf("pwd: too many arguments\n");
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
        return 1;
    }

    printf("%s\n", cwd);

    return 1;
}

int builtin_cd(char **args)
{
    char *path;

    if (args[1] == NULL) {
        path = getenv("HOME");

        if (path == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }
    else {
        path = args[1];
    }

    if (args[2] != NULL) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    if (chdir(path) != 0) {
        perror("cd");
    }

    return 1;
}

int builtin_exit(char **args)
{
    if (args[1] != NULL) {
        fprintf(stderr, "exit: too many arguments\n");
        return 1;
    }

    exit(0);
}

int builtin_execute(char **args)
{
    if (args == NULL || args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "pwd") == 0) {
        return builtin_pwd(args);
    }

    if (strcmp(args[0], "cd") == 0) {
        return builtin_cd(args);
    }

    if (strcmp(args[0], "exit") == 0) {
        return builtin_exit(args);
    }

    return 0;
}
