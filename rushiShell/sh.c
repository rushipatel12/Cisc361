#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
						  out with*/
     
  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  while ( go )
  {
      /* print your prompt */
    printf("[%s]%s",pwd,prompt);
    /* get command line and process */
    arg = fgets(commandline,BUFSIZ,stdin);
    int argLen = strlen(arg);
    arg[argLen-1]=0;
    args = stringToArray(arg);
    i=0;
    argsct=0;
    while(args[i]!=NULL){
      argsct++;
      i++;
    }
    // printf("%s-%s",args[0],args[1]);
    if((strcmp(args[0],"exit")==0) && argsct==1){
      go = 0;
    }
    // else{
    //   char *ab = get_path(cmd[0]);
    //   if(ab == NULL){
    //     printf("Command not found: %c\n",cmd[0]);
    //   }
    //   else{
    //     int pid = fork();
    //     if(pid == 0){
    //       exec(ab,cmd);
    //       printf("exited");
    //       exit(2);
    //     }
    //     else{
    //       waitpid(pid,NULL,NULL);
    //     }
    //   }
    // }

    /* check for each built in command and implement */

     /*  else  program to exec */
       /* find it */
       /* do fork(), execve() and waitpid() */

      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    //which
    if (strcmp(args[0], "which") == 0){
      if (argsct == 1){
        fprintf(stderr, "which: Too few arguments.\n");
      }
      else{
        printf("Executing built-in which\n");
        int i = 1;
        while (i < argsct && i < MAXARGS){ //execute for all arguments
          char *cmd = which(args[i], pathlist);
          printf("%s", cmd);
          i++;
          free(cmd);
        }
      }
    } //where
    else if (strcmp(args[0], "where") == 0){
      if (argsct == 1){
        fprintf(stderr, "where: Too few arguments.\n");
      }
      else{
        printf("Executing built-in where\n");
        int i = 1;
        while (i < argsct && i < MAXARGS){ //execute for all arguments
          char *cmd = where(args[i], pathlist);
          printf("%s", cmd);
          i++;
          free(cmd);
        }
      }
    }
    freeArgs(args);
  }

  free(args);
  free(prompt);
  free(commandline);
  free(pwd);
  free(owd);
  free(pathlist->element);
  freeList(pathlist);
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
  char *cmd = malloc(64);
  int found = 0; //used to tell whether a path was found or not

  //check to see if built in command
  if ((strcmp(command, "which") == 0) ||
      (strcmp(command, "where") == 0) ||
      (strcmp(command, "cd") == 0) ||
      (strcmp(command, "pwd") == 0) ||
      (strcmp(command, "list") == 0) ||
      (strcmp(command, "pid") == 0) ||
      (strcmp(command, "kill") == 0) ||
      (strcmp(command, "prompt") == 0) ||
      (strcmp(command, "printenv") == 0) ||
      (strcmp(command, "setenv") == 0)){
        sprintf(cmd, "%s: shell built-in command.\n", command);
        return cmd;
      }

  //else, locate command
  while (pathlist) {
    sprintf(cmd, "%s/%s", pathlist->element, command);
    if (access(cmd, X_OK) == 0) {
      sprintf(cmd, "%s/%s\n", pathlist->element, command);
      found = 1;
      break;
    }
    pathlist = pathlist->next;
  }
  //check if command not found
  if (found == 0){
    sprintf(cmd, "%s: Command not found.\n", command);
    return cmd;
  }
  return cmd;
} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
  char *cmd = calloc(64, sizeof(char*));
  char *test = calloc(256, sizeof(char*));
  int found = 0; //used to tell whether a path was found or not
  int built_in = 0; //used to tell whether command is built in or not

  //check to see if built in command
  if ((strcmp(command, "which") == 0) ||
      (strcmp(command, "where") == 0) ||
      (strcmp(command, "cd") == 0) ||
      (strcmp(command, "pwd") == 0) ||
      (strcmp(command, "list") == 0) ||
      (strcmp(command, "pid") == 0) ||
      (strcmp(command, "kill") == 0) ||
      (strcmp(command, "prompt") == 0) ||
      (strcmp(command, "printenv") == 0) ||
      (strcmp(command, "setenv") == 0)){
        sprintf(cmd, "%s: shell built-in command.\n", command);
        built_in = 1;
      }

  //else, locate command
  while (pathlist) {
    sprintf(test, "%s/%s", pathlist->element, command);
    if (access(test, X_OK) == 0){
      char *toAppend = malloc(64);
      sprintf(toAppend, "%s%s\n", cmd, test);
      sprintf(cmd, "%s", toAppend);
      //sprintf(cmd, "%s%s/%s\n", cmd, pathlist->element, command);
      found = 1;
      free(toAppend);
    }
    pathlist = pathlist->next;
  }
  free(test);
  if (found == 0 && built_in == 0){
    sprintf(cmd, "%s: Command not found.\n", command);
    return cmd;
  }
  return cmd;


} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */

void freeList(struct pathelement *first){
  struct pathelement* tmp = NULL;
   while (first != NULL)
    {
       tmp = first;
       first = first->next;
       free(tmp);
    }

}

void freeArgs(char **array){
  int i = 0;
  while(array[i] != NULL){
    free(array[i]);
    i++;
  }
}

char **stringToArray(char *input){
  char buf[BUFSIZ];
  strcpy(buf,input);
  char *t = strtok(buf," ");
  int count=1;
  while(strtok(NULL," ")){
    count++;
  }
  char **array = malloc((count+1)*sizeof(char*));
  array[count]= 0;

  count = 0;
  strcpy(buf,input);
  t=strtok(buf," ");
  while(t){
    int len = strlen(t);
    array[count] = (char *) malloc((len+1)*sizeof(char));
    strcpy(array[count],t);
    count++;
    t=strtok(NULL," ");
  }
  return array;
}