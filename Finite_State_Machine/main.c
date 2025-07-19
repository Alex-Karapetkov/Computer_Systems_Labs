/*
 * CS 361: Template lab driver
 *
 * Name: Alex Karapetkov
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "procmodel.h"
#include "statemodel.h"

void
usage (void)
{
  printf ("fsm, a state model simulator\n\n");
  printf ("usage: fsm [evt1 evt2 ...]\n");
  printf (
      "evt1 evt2 ... is a sequence of event combinations. Each event is\n");
  printf ("represented by an int value:\n\n");
  printf ("  ADMIT = 0\n");
  printf ("  DISPATCH = 1\n");
  printf ("  SCHEDULE = 2\n");
  printf ("  BLOCK = 3\n");
  printf ("  UNBLOCK = 4\n");
  printf ("  EXIT = 5\n");
  printf ("  KILL = 6\n");
  printf ("  NIL = 7\n");
}

int
main (int argc, char **argv)
{
  // With no arguments, show usage and exit
  if (argc < 2)
    {
      usage ();
      return EXIT_FAILURE;
    }

  // Declare and initialize a FSM
  fsm_t *process = process_init ();
  if (process == NULL)
    return EXIT_FAILURE;

  // EXAMPLE: Send ADMIT and DISPATCH to the process.
  // TODO: Replace this with a loop for all events.
  /*
  handle_event (process, ADMIT);
  assert (process->state == RDY);
  handle_event (process, DISPATCH);
  assert (process->state == RUN);
  */

  for (int i = 1; i < argc; i++)
    {
      // check for bad arguments
      if (argv[i] == NULL)
        {
          return EXIT_FAILURE;
        }

      long string_num = strtol (argv[i], NULL, 10);

      if (string_num == 0)
        {
          handle_event (process, ADMIT);
          assert (process->state == RDY);
        }
      else if (string_num == 1)
        {
          handle_event (process, DISPATCH);
          assert (process->state == RUN);
        }
      else if (string_num == 2)
        {
          handle_event (process, SCHEDULE);
          assert (process->state == RDY);
        }
      else if (string_num == 3)
        {
          handle_event (process, BLOCK);
          assert (process->state == BLK);
        }
      else if (string_num == 4)
        {
          handle_event (process, UNBLOCK);
          assert (process->state == RDY);
        }
      else if (string_num == 5)
        {
          handle_event (process, EXIT);
        }
      else if (string_num == 6)
        {
          handle_event (process, KILL);
          assert (process->state == TRM);
        }
      else if (string_num == 7)
        {
          handle_event (process, NIL);
        }
      else
        {
          usage ();
          return EXIT_FAILURE;
        }
    }

  // Under normal runs, we should always end in TRM state
  if (process->state != TRM)
    {
      free (process);
      return EXIT_FAILURE;
    }

  free (process);
  return EXIT_SUCCESS;
}
