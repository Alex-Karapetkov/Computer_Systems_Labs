#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "effects.h"
#include "procmodel.h"
#include "statemodel.h"

/* Generic front-end for handling events. Should do nothing more
   than calling the appropriate *_event function based on the type. */
void
handle_event (fsm_t *fsm, event_t event)
{
  // perform robust error handling for invalid events and transitions
  // use assert() to ensure that the passed fsm parameter is never NULL
  // change state only if the table indicates that the next state is valid
  // (something other than NST or a negative value)

  // ensures that the passed fsm parameter is never NULL
  assert (fsm != NULL);

  // Validate that event is correct for FSM
  if (event >= fsm->nevents)
    return;

  // use the FSM transition lookup function to obtain effect and new state's
  // entry action (if either or both are defined)
  action_t effect = NULL;
  action_t entry = NULL;
  state_t next = fsm->transition (fsm, event, &effect, &entry);

  // keep track of old state before switching to new one for print statement
  state_t old_state;

  // if next state is -1, the event is not defined for the current state
  if (next == -1)
    {
      next = NST;
      old_state = fsm->state;
    }

  if (next != NST)
    {
      old_state = fsm->state;
      fsm->state = next;
    }

  // Event and state names for printing out
  static const char *event_names[]
      = { "ADMIT",   "DISPATCH", "SCHEDULE", "BLOCK",
          "UNBLOCK", "EXIT",     "KILL",     "NIL" };
  static const char *state_names[]
      = { "NEW", "RDY", "RUN", "BLK", "TRM", "NST" };
  // TODO: Use the following format string to print the current
  // state name, state name, the event name, and the new state
  // name for valid transitions.
  // printf ("%s.%s -> %s\n", ...);

  printf ("%s.%s -> %s\n", state_names[old_state], event_names[event],
          state_names[next]);

  // perform the effect if it is defined
  if (effect != NULL)
    effect (fsm);

  // perform the new state's entry action if it is defined
  if (entry != NULL)
    entry (fsm);
}
