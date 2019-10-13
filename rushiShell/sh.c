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
  char *command, *commandpath, *p, *pwd, *owd;
  char **args=NULL;
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
  // pathlist = get_path();

  while ( go )
  {
    pathlist = get_path();
      /* print your prompt */
    printf("[%s]%s",pwd,prompt);
    /* get command line and process */
    char *arg = calloc(MAX_CANON, sizeof(char));
    arg = fgets(arg,BUFSIZ,stdin);
    if (arg == NULL){ //cntrl D
      printf("^D\n");
      free(pathlist->element);
      freeList(pathlist);
      free(arg);
      continue;
    }
    else if((strcmp(arg, "\n") == 0)){ //enter key
      free(pathlist->element);
      freeList(pathlist);
      free(arg);
      continue;
    }
    else{
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
    else if (strcmp(args[0], "cd") == 0){
      if (argsct > 2){
        fprintf(stderr, "cd: Too many arguments.\n");
      }
      else{
        int success = 0;
        printf("Executing built-in cd\n");
        if (argsct == 1){ //no arguments
          success = cd(homedir);
        }
        else if (strcmp(args[1], "-") == 0){ //go to previous directory
          success = cd(owd);
        }
        else{
          success = cd(args[1]);
        }
        if (success){ //change owd and cwd if cd worked
          free(owd);
          owd = pwd;
          pwd = getcwd(NULL, PATH_MAX+1);
        }
      }
    }

     //pwd
    else if (strcmp(args[0], "pwd") == 0){
      printf("Executing built-in pwd\n");
      if (argsct > 1){
        fprintf(stderr, "pwd: ignoring non-option arguments\n");
      }
      char *wd = getcwd(NULL, PATH_MAX+1);
      printf("%s\n", wd);
      free(wd);
    }

        //kill
    else if (strcmp(args[0], "kill") == 0){
      if (argsct > 3){
        fprintf(stderr, "kill: Too many arguments.\n");
      }
      else if(argsct == 1){
        fprintf(stderr, "kill: Too few arguments.\n");
      }
      else{
        printf("Executing built-in kill\n");
        if (argsct == 2){ //no specified signal
          int pid = atoi(args[1]);
          mykill(pid, 15);
        }
        else{
          int pid = atoi(args[2]);
          int signal = atoi(args[1]);
          signal = signal * -1;
          mykill(pid, signal);
        }
      }
    }
        //list
    else if (strcmp(args[0], "ls") == 0){
      printf("Executing built-in list\n");
      if (argsct == 1){ //no arguments
        list(pwd);
      }
      else{
        int i = 1;
        while (i < argsct && i < MAXARGS){
          list(args[i]);
          printf("\n");
          i++;
        }
      }
    }


     //prompt
    else if (strcmp(args[0], "prompt") == 0){
      if (argsct > 2){
        fprintf(stderr, "prompt: too many arguments.\n");
      }
      else{
        printf("Executing built-in prompt\n");
        if (argsct == 1){ //no arguments, input prompt
          printf("Input prompt prefix: ");
          if (!fgets(prompt, BUFSIZ, stdin))
            fprintf(stderr, "fgets error\n");
          size_t p_len = strlen(prompt);
          if (prompt[p_len - 1] == '\n') //change /n to /0
            prompt[p_len - 1] = '\0';
        }
        else if (argsct == 2){
          sprintf(prompt, "%s", args[1]);
        }
      }
    }
    //pid
    else if (strcmp(args[0], "pid") == 0){
      printf("Executing built-in pid\n");
      if (argsct > 1){
        fprintf(stderr, "pid: ignoring non-option arguments.\n");
      }
      printPid();
    }



    //printenv
      else if (strcmp(args[0], "printenv") == 0)
      {
        printenv(argsct, envp, args);
      }

      //setenv
      else if (strcmp(args[0], "setenv") == 0)
      {
        //Print env if zero args
        if (argsct == 1)
        {
          printenv(argsct, envp, args);
        }
        else if (argsct == 2)
        {
          //Set to empty
          setenv(args[1], "", 1);
        }
        else if (argsct == 3)
        {
          //Reset vars
          setenv(args[1], args[2], 1);

          //special care for home and path
          if (strcmp(args[1], "HOME") == 0)
          {
            homedir = getenv("HOME");
          }
          else if (strcmp(args[1], "PATH") == 0)
          {
            pathlist = get_path();
          }
        }
        else
        {
          printf("%s\n", "setenv: Incorrect amount of arguments");
        }
        break;
      }
    // else{
    //   struct pathelement *ab = get_path(args[0]);
    //   pid_t pid;
    //   if(ab == NULL){
    //     printf("Command not found: %s\n",args[0]);
    //   }
    //   else{
    //     pid = fork();
    //     if(pid == 0){
    //       execve(args[0],&args[0], NULL);
    //       printf("exited");
    //       exit(pid);
    //     }
    //     else{
    //       waitpid(pid,NULL,0);
    //     }
    //   }
    //   mykill(pid, 15);
    //   free(ab->element);
    //   freeList(ab);
    // }
    freeArgs(args);
    free(args);
    free(pathlist->element);
    freeList(pathlist);
  }
  free(arg);
  }
  free(prompt);
  free(pwd);
  free(owd);
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
  char *cmd = malloc(64);
  int found = 0; //used to tell whether a path was found or not


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

// void list ( char *dir )
// {
//   /* see man page for opendir() and readdir() and print out filenames for
//   the directory passed */
// } /* list() */

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
  // free(t);
  return array;
}

void list ( char *dir )
{
  DIR *mydir;
  struct dirent *file;
  if ((mydir = opendir(dir)) == NULL){
    printf("list: cannot access ''%s' : no such file or directory ", dir);
  }
  else{
    printf("%s:\n", dir);
    while ((file = readdir(mydir)) != NULL){
      printf("%s ", file->d_name);
    }
    printf("\n");
    free(file);
  }
  free(mydir);
}

int cd(char *directory){ //return an int to tell if chdir worked
  if (chdir(directory) < 0){
    printf("%s: no such file or directory\n", directory);
    return 0;
  }
  return 1;
}

void mykill(int pid, int signal){ //kills pid with signal
  kill(pid, signal);
}
void printPid(){ //prints out current process pid
  printf("%d\n", getpid());
}

void printenv(int num_args, char **envp, char **args){
  if (num_args == 1)
  {
    int i = 0;
    while (envp[i] != NULL)
    {
      printf("%s\n", envp[i]);
      i++;
    }
  }
  else if (num_args == 2)
  {
    char *env_str = getenv(args[1]);
    if (env_str != NULL)
    {
      printf("%s\n", env_str);
    }
  }
}