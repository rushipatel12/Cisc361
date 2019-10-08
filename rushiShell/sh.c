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
    char *cmd;
      /* print your prompt */
    printf("[%s]%s",pwd,prompt);
    /* get command line and process */
    fgets(commandline,BUFSIZ,stdin);
    cmd = strtok(commandline, " ");
    if(strcmp(&cmd[0],"exit")==0){
      go = 0;
    }
    else{
      // char *ab = get_path(cmd[0]);
      // if(ab == NULL){
      //   printf("Command not found: %c\n",cmd[0]);
      // }
      // else{
      //   int pid = fork();
      //   if(pid == 0){
      //     exec(ab,cmd);
      //     printf("exited");
      //     exit(2);
      //   }
      //   else{
      //     waitpid(pid,NULL,NULL);
      //   }
      // }
    }

    /* check for each built in command and implement */

     /*  else  program to exec */
    {
       /* find it */
       /* do fork(), execve() and waitpid() */

      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    }
  
  free(args);
  free(prompt);
  free(commandline);
  free(pwd);
  free(owd);
    // struct pathelement *tmpFree = pathlist;
    // while(tmpFree->next != NULL){
    //   free(pathlist->element);
    // }
  free(pathlist->element);
  free(pathlist->next);
  free(pathlist);
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */

  struct pathelement *tmp = pathlist;
  while(tmp->next != NULL){
    if(strcmp(tmp->element,command)==0){
      return command;
    }
    else{
      return NULL;
    }
  }
  {
    /* data */
  };
  return NULL;

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
  struct pathelement *tmp = pathlist;
  while(tmp->next != NULL){
    if(strcmp(tmp->element,command)==0){
      return command;
    }
    else{
      return NULL;
    }
  }

  return NULL;
  

} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */

