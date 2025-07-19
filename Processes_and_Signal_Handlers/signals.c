#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "signals.h"

static void catch_loop (void);
static void catch_segfault (void);

/* Convert the short signal name (NNN part of SIGNNN) to its number.
   Consult /usr/include/asm/signal.h for the number mappings. For
   this lab, hard-code the numbers; do not use the numeric constants.
   Return -1 is the name isn't one you need to look up. */
int
signal_lookup (char *name)
{
  // TODO: Look up the numbers for INT, SEGV, KILL, and TSTP
  if (!strcmp ("INT", name))
    return 2;
  else if (!strcmp ("SEGV", name))
    return 11;
  else if (!strcmp ("KILL", name))
    return 9;
  else if (!strcmp ("TSTP", name))
    return 20;
  else
    return -1;
}

// Entry point for overriding a signal handler in this lab.
void
override (char *signal)
{
  int sig = signal_lookup (signal);
  if (sig == SIGINT)
    catch_loop ();
  else if (sig == SIGSEGV)
    catch_segfault ();
  else
    printf ("Received bad signal name: %s\n", signal);
}

// Signal handler for SIGINT. This function doesn't use the signal number.
void
sigint_handler (int signum __attribute__ ((unused)))
{
  // TODO: Complete this signal handler so that it writes the message
  // "Breaking out of the loop\n" and exits with return code 0. Note
  // that you cannot use printf() in a signal handler. You must use
  // write().

  char buffer[] = "Breaking out of the loop\n";
  write (STDOUT_FILENO, buffer, sizeof (buffer));
  exit (0);
}

void
catch_loop (void)
{
  // TODO: Override the signal handler for interrupts (SIGINT).
  // Then create a child with the following code:
  //   printf ("Child has started\n");
  //   while (1) ;

  // After starting the child, the parent will call sleep (1) to wait
  // for 60 seconds before sending the SIGINT signal to the child.
  // The parent must wait for the child to run (and exit), then print
  // "Child was stopped\n"

  /* Creates a sigaction and links it to the handler */
  struct sigaction sa;
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = sigint_handler;

  /* Overwrites the SIGINT handler */
  if (sigaction (SIGINT, &sa, NULL) == -1)
    exit (EXIT_FAILURE);

  pid_t child_pid = fork ();
  if (child_pid < 0)
    exit (EXIT_FAILURE);

  if (child_pid == 0)
    {
      printf ("Child has started\n");
      while (1)
        ;
    }

  /* Parent calls sleep to wait for 60 seconds then sends SIGINT signal to
     child. Parent must wait for child to run and exit then print "Child was
     stopped\n" */
  sleep (1);
  kill (child_pid, SIGINT);
  wait (NULL);
  printf ("Child was stopped\n");
}

// TODO: Create a signal handler for handling segmentation faults.
// Inside this function, you must use snprintf() to create the
// following output string (the %d is for the signal number):
//   "You received signal %d\n"
// After creating the buffer, use write() to print this to STDOUT.
// (Recall that printf() is NOT allowed in signal handlers.)
// Finally, exit with return code 37.

void
sigsegv_handler (int signum)
{
  // length is 24 to include length of output string + the null byte
  char buffer[24];

  snprintf (buffer, sizeof (buffer), "You received signal %d\n", signum);
  write (STDOUT_FILENO, buffer, sizeof (buffer));
  exit (37);
}

void
catch_segfault (void)
{
  // TODO: Override the signal handler for segmentation faults (SIGSEGV).
  // Then create a child with the following code:
  //   int *ptr = NULL;
  //   *ptr = 5;
  //   printf ("FAIL: This should never be reached\n");

  // The parent must wait for the child to run (and exit), then get the
  // child's exit status, printing:
  //   "Child exited with status %d\n"

  int child_status = 0;

  // create a sigaction and link it to the handler
  struct sigaction sa;
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = sigsegv_handler;

  // overwrite the SIGSEGV handler
  if (sigaction (SIGSEGV, &sa, NULL) == -1)
    exit (EXIT_FAILURE);

  pid_t child_pid = fork ();
  if (child_pid < 0)
    exit (EXIT_FAILURE);

  if (child_pid == 0)
    {
      int *ptr = NULL;
      *ptr = 5;
      printf ("FAIL: This should never be reached\n");
    }

  wait (&child_status);
  printf ("Child exited with status %d\n", WEXITSTATUS (child_status));
}