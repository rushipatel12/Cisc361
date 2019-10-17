#include "sh.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal); 

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */

  struct sigaction sig = {0};
  sig.sa_handler = SIG_IGN;
  signal(SIGINT, sig_handler);
  sigaction(SIGTERM, &sig, NULL);
  sigaction(SIGTSTP, &sig, NULL);

  return sh(argc, argv, envp);
}

void sig_handler(int signal)
{
  /* define your signal handler */
  printf("\nsignal detected [%d]\n", signal);
}

