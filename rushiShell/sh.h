
#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list(char *directory);
void listFree(struct pathelement *first);
void arrayFree(char **array);
char **stringToArray(char *input);
int cd(char *directory);
void printPid();
void printenv(int argsct, char **envp, char **args);


#define PROMPTMAX 32
#define MAXARGS 10
