#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "child.h"

pid_t
run_child (void)
{
  char *str = "I am the parent";
  int status = 0;

  // TODO: Add code here to create a child process that prints
  // "I am the child" and exits with return code 42. The parent
  // must wait for the child to finish before printing the
  // child's exit status (consult the textbook for this as needed).
  // The parent should return the child's PID from this function.

  pid_t child_pid = fork ();
  if (child_pid < 0)
    exit (EXIT_FAILURE);

  if (child_pid == 0)
    {
      printf ("Child starting\n");
      printf ("I am the child\n");
      exit (42);
    }

  wait (&status);
  printf ("Child exited with status %d\n", WEXITSTATUS (status));

  fprintf (stderr, "str [at %p]: %p\n", (void *)&str, (void *)str);
  printf ("%s\n", str);
  return child_pid;
}
