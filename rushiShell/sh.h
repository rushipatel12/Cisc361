
#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list ( char *dir );
void printenv(char **envp);
void freeList(struct pathelement *first);
void freeArgs(char **array);
char **stringToArray(char *input);
int cd(char *directory);
void mykill(int pid, int signal);

#define PROMPTMAX 32
#define MAXARGS 10
