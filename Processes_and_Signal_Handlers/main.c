/*
 * CS 361: Process management lab
 *
 * Name: Alex Karapetkov
 */

#include <assert.h>
#include <getopt.h>
#include <inttypes.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "child.h"
#include "signals.h"

#define SIGNAL_MAX 20

int cmdline (int, char **, char **);

void
usage (void)
{
  printf ("Usage: procs [option]\n");
  printf (" Options are:\n");
  printf ("  -s N    Override signal number N and run signal handler\n");
  printf ("\n Default is to create a child process with normal exit\n");
}

int
main (int argc, char **argv)
{
  char *signal_override = NULL;
  if (cmdline (argc, argv, &signal_override) < 0)
    {
      usage ();
      return EXIT_FAILURE;
    }

  // Set up a timeout to kill the process after 5 seconds
  struct itimerval timeout;
  memset (&timeout, 0, sizeof (timeout));
  timeout.it_value.tv_sec = 5;
  timeout.it_value.tv_usec = 0;
  setitimer (ITIMER_REAL, &timeout, NULL);

  if (signal_override == NULL)
    run_child ();
  else
    override (signal_override);

  return EXIT_SUCCESS;
}

/* DO NOT MODIFY THIS FUNCTION. Parses the command line arguments to get
   the portion of the lab to run. */
int
cmdline (int argc, char **argv, char **sig)
{
  int option;

  while ((option = getopt (argc, argv, "s:")) != -1)
    {
      switch (option)
        {
        case 's':
          *sig = optarg;
          break;
        default:
          return -1;
        }
    }

  return 0;
}
