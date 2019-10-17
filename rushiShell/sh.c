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
#include <glob.h>

extern char **environ;

int sh( int argc, char **argv, char **envp ){
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
     
  if ((pwd = getcwd(NULL, PATH_MAX+1)) == NULL ){
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  // pathlist = get_path();

  while (go){
    pathlist = get_path();
    /* print your prompt */
    printf("[%s]%s",pwd,prompt);
    /* get command line and process */
    char *arg = calloc(MAX_CANON, sizeof(char));
    arg = fgets(arg,BUFSIZ,stdin);
    if (arg == NULL){ //cntrl D
      printf("^D\n");
    }
    else if((strcmp(arg, "\n") == 0)){ //enter key
      free(pathlist->element);
      listFree(pathlist);
      free(arg);
      continue;
    }
    else{
      int argLen = strlen(arg);
      arg[argLen-1]=0;
      args = stringToArray(arg);
      i=0;
      argsct=0;
      char *tmp;
      tmp = &args[0][0];
      while(args[i]!=NULL){
        argsct++;
        i++;
      }
      if((strcmp(args[0],"exit")==0) && argsct==1){
        go = 0;
      }

      //which
      else if (strcmp(args[0], "which") == 0){
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
      } 
      
      //where
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
      //list
      else if (strcmp(args[0], "ls") == 0 || strcmp(args[0], "list") == 0){
        printf("Executing built-in list\n");
        if (argsct == 1){ //no arguments
          ls(pwd);
        }
        else{
          int i = 1;
          while (i < argsct && i < MAXARGS){
            ls(args[i]);
            printf("\n");
            i++;
          }
        }
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
            arrayFree(args);
            free(args);
            free(pathlist->element);
            listFree(pathlist);
            free(arg);
            free(prompt);
            free(pwd);
            free(owd);
            kill(pid,SIGTERM);
          }
          else{
            int pid = atoi(args[2]);
            int signal = atoi(args[1]);
            signal = signal * -1;
            kill(pid, signal);
          }
        }
      }

      //setenv
      else if(strcmp(args[0], "setenv") == 0){
        if (argsct > 3){
          fprintf(stderr, "setenv: Too many arguments.\n");
        }
        else{
          printf("Executing buit-in setenv.\n");
          if (argsct == 1){
            printenv(argsct, environ, args);
          }
          else if(argsct == 2){ //set args[1] to environment variable as " "
            setenv(args[1], " ", 1);
            if (strcmp(args[1], "HOME") == 0){ //change homedir
              homedir = " ";
            }
          }
          else if (argsct == 3){ //set environment variable args[1] to args[2]
            setenv(args[1], args[2], 1);
            if (strcmp(args[1], "HOME") == 0){ //change homedir
              homedir = args[2];
            }
          }
        }
      }

      //cd
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
            // free(owd);
            owd = pwd;
            pwd = getcwd(NULL, PATH_MAX+1);
          }
        }
      }

      //printenv
      else if (strcmp(args[0], "printenv") == 0){
        printenv(argsct, envp, args);
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

      //pid
      else if (strcmp(args[0], "pid") == 0){
        printf("Executing built-in pid\n");
        if (argsct > 1){
          fprintf(stderr, "pid: ignoring non-option arguments.\n");
        }
        printPid();
      }

      //running programs and not built in commands
      else{
        struct pathelement *ab = get_path(args[0]);
        int pid;
        pid = fork();
        if(pid == 0){
          char *cmd_path;

          if(args[0][0] == '.' || args[0][0] == '/'){
            cmd_path = (char *) malloc((strlen(args[0])+1)*sizeof(char));
            strcpy(cmd_path,args[0]);
          }
          else{
            cmd_path = which(args[0],pathlist);
          }

          int access_result = access(cmd_path, F_OK | X_OK);
          execve(args[0],&args[0], NULL);
          printf("exited");
          free(ab->element);
          listFree(ab);
          arrayFree(args);
          free(args);
          free(pathlist->element);
          listFree(pathlist);
          free(arg);
          free(prompt);
          free(pwd);
          free(owd);
          free(cmd_path);
          exit(pid);
          
        }
        else if(pid != 0){
          waitpid(pid,NULL,0);
        }
        else{
          printf("Command not found: %s\n",args[0]);

        }
        free(ab->element);
        listFree(ab);
      }
      arrayFree(args);
      free(args);
      free(pathlist->element);
      listFree(pathlist);
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

char *where(char *command, struct pathelement *pathlist ){
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
      (strcmp(command, "ls") == 0) ||
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

void ls(char *directory){
  DIR *mydirectory;
  struct dirent *file;
  if ((mydirectory = opendir(directory)) == NULL){
    printf("list: cannot access ''%s' : no such file or directory ", directory);
  }
  else{
    printf("%s:\n", directory);
    while ((file = readdir(mydirectory)) != NULL){
      printf("%s ", file->d_name);
    }
    printf("\n");
    free(file);
  }
  free(mydirectory);
}

void printenv(int argsct, char **envp, char **args){
  if (argsct == 1)
  {
    int i = 0;
    while (envp[i] != NULL)
    {
      printf("%s\n", envp[i]);
      i++;
    }
  }
  else if (argsct == 2)
  {
    char *env_str = getenv(args[1]);
    if (env_str != NULL)
    {
      printf("%s\n", env_str);
    }
  }
}

void printPid(){ //prints out current process pid
  printf("%d\n", getpid());
}

int cd(char *directory){ //return an int to tell if chdir worked
  if (chdir(directory) < 0){
    printf("%s: no such file or directory\n", directory);
    return 0;
  }
  return 1;
}

void listFree(struct pathelement *first){
  struct pathelement* tmp = NULL;
   while (first != NULL)
    {
       tmp = first;
       first = first->next;
       free(tmp);
    }

}

void arrayFree(char **array){
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
