#include <fcntl.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipe.h"

/* Given a writable string buffer, find the first space and replace it
   with a '\0', thus ending the string at that location. Sample call:

     char buffer[] = "hello world";
     char *result = split_string (buffer);
     // result points to buffer, which prints as "hello"
   */
char *
split_string (char *buffer)
{
  if (buffer == NULL)
    {
      exit (EXIT_FAILURE);
    }

  int space = ' ';
  char *first_space = strchr (buffer, space);

  if (first_space == NULL)
    {
      exit (EXIT_FAILURE);
    }

  *first_space = '\0';
  return buffer;
}

/* Creates a child process. If this fails, close the pipe and return NULL.
   If the child is created, use it to run the cksum program on the filename
   passed. Send the result back through the pipe. The parent process should
   just return the child's PID. Sample call:

     int fd[2];
     pipe (fd);
     pid_t child_pid = create_cksum_child (fd, "foo.txt");
  */
pid_t
create_cksum_child (int *pipe, char *const filename)
{
  pid_t child_pid = fork ();
  if (child_pid == -1)
    {
      close (pipe[0]);
      close (pipe[1]);
      return 0;
    }

  if (child_pid == 0)
    {
      close (pipe[0]);
      dup2 (pipe[1], STDOUT_FILENO);
      execlp ("cksum", "cksum", filename, NULL);
    }

  return child_pid;
}

/* Uses the cksum program on the input filename. Start by creating a pipe
   and calling create_cksum_child() to create the child process. If that
   fails, return NULL. Otherwise, implement the parent here so that it
   reads the result of the cksum calculation from the pipe and returns
   the result. Sample call:

     char *sum = get_cksum ("data/f1.txt");
     // sum is "3015617425 6 data/f1.txt\n" [with the newline]
   */
char *
get_cksum (char *const filename)
{
  int pipefd[2];
  pipe (pipefd);
  pid_t child_pid = create_cksum_child (pipefd, filename);

  if (child_pid == 0)
    {
      return NULL;
    }

  close (pipefd[1]);

  char *buffer = calloc (100, sizeof (char));
  read (pipefd[0], buffer, 100);

  return buffer;
}

/* Re-implementation of the create_cksum_child() in pipe.c. Instead of
   using the calls to pipe/fork/dup2/exec, combine the latter three
   into a call to posix_spawn(). Sample call:

     char *sum = spawn_cksum ("data/f1.txt");
     // sum is "3015617425 6 data/f1.txt\n" [with the newline]
   */
char *
spawn_cksum (char *const filename)
{
  // Use these parameters to posix_spawn(). You will need to set up
  // the posix_spawn_file_actions_t by (1) initializing it, (2) adding
  // a close action, and (3) adding a dup2 action so that STDOUT writes
  // to a pipe. After spawning the new process, make sure to call
  // posix_spawn_file_actions_destroy() on the actions to prevent
  // memory leaks.

  // set up pipe
  int pipefd[2];
  pipe (pipefd);

  // set up posix_spawn_file_actions_t
  posix_spawn_file_actions_t actions;
  posix_spawn_file_actions_init (&actions);
  posix_spawn_file_actions_addclose (&actions, pipefd[0]);
  posix_spawn_file_actions_adddup2 (&actions, pipefd[1], STDOUT_FILENO);

  const char *path = "/usr/bin/cksum";
  char *const argv[] = { "cksum", filename, NULL };

  // Instead of using fork() and exec(), use the posix_spawn functions.
  // Note that you will need to add a close and dup2 action to the
  // set of file actions to run when spawning. You will also need to
  // create the pipe to use with the dup2 action.

  // spawn new process using posix_spawn
  pid_t child_pid = 0;
  posix_spawn (&child_pid, path, &actions, NULL, argv, NULL);
  // destroy file_actions
  posix_spawn_file_actions_destroy (&actions);
  // Parent code: read the value back from the pipe into a dynamically
  // allocated buffer. Wait for the child to exit, then return the
  // buffer.
  char *buffer = calloc (100, sizeof (char));
  read (pipefd[0], buffer, 100);
  wait (NULL);

  return buffer;
}
