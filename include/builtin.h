#ifndef BUILTIN_H
#define BUILTIN_H

int builtin_execute(char **args);

int builtin_cd(char **args);
int builtin_pwd(char **args);
int builtin_echo(char **args);
int builtin_exit(char **args);

#endif
