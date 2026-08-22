#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"


/*
 * BUILTIN: cd
 *
 * cd
 *     -> go to HOME
 *
 * cd <directory>
 *     -> change to directory
 *
 * More than one argument -> error
 */
int builtin_cd(char **args)
{
    char *dir;

    /* cd with no argument */
    if (args[1] == NULL) {

        dir = getenv("HOME");

        if (dir == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }

    /* cd with one argument */
    else if (args[2] == NULL) {

        dir = args[1];
    }

    /* cd with more than one argument */
    else {

        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    /* Change directory */
    if (chdir(dir) != 0) {
        perror("cd");
    }

    return 1;
}


/*
 * BUILTIN: pwd
 *
 * Prints current working directory.
 */
int builtin_pwd(char **args)
{
    char cwd[4096];

    /* pwd should not have arguments */
    if (args[1] != NULL) {
        fprintf(stderr, "pwd: too many arguments\n");
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
        return 1;
    }

    printf("%s\n", cwd);

    return 1;
}


/*
 * BUILTIN: echo
 *
 * Prints all arguments after echo.
 */
int builtin_echo(char **args)
{
    int i = 1;

    while (args[i] != NULL) {

        printf("%s", args[i]);

        if (args[i + 1] != NULL) {
            printf(" ");
        }

        i++;
    }

    printf("\n");

    return 1;
}


/*
 * BUILTIN: exit
 *
 * Terminates ShellForge.
 *
 * No "exiting..." message.
 */
int builtin_exit(char **args)
{
    if (args[1] != NULL) {
        fprintf(stderr, "exit: too many arguments\n");
        return 1;
    }

    exit(0);
}


/*
 * Check and execute built-in commands.
 */
int builtin_execute(char **args)
{
    if (args == NULL || args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {
        return builtin_cd(args);
    }

    if (strcmp(args[0], "pwd") == 0) {
        return builtin_pwd(args);
    }

    if (strcmp(args[0], "echo") == 0) {
        return builtin_echo(args);
    }

    if (strcmp(args[0], "exit") == 0) {
        return builtin_exit(args);
    }

    return 0;
}
