#include "sh.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int sig_num); 

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */
  signal(SIGINT, sig_handler);
  signal(SIGTSTP, sig_handler);
  signal(SIGQUIT, sig_handler);
  signal(SIGTERM, sig_handler);

  return sh(argc, argv, envp);
}

void sig_handler(int sig_num)
{
  /* define your signal handler */
  signal(SIGINT, sig_handler);
  signal(SIGTSTP, sig_handler);
  signal(SIGQUIT, sig_handler);
  signal(SIGTERM, sig_handler);
  printf("\nIgnored Signal\n");
}

