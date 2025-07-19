#include <stdlib.h>

#include "effects.h"
#include "procmodel.h"
#include "statemodel.h"

/* Prototype for actions/effects and transition function */
static state_t parse_transition (fsm_t *, event_t, action_t *, action_t *);

/* Initialize FSM to contain pointers to transition table, effect table,
   and entry actions. Also set the live, runtime, and state values to
   defaults. Return true if successful. Return false if anything fails
   or cannot be set successfully. */
fsm_t *
process_init (void)
{
  fsm_t *fsm = calloc (1, sizeof (fsm_t));
  fsm->state = NEW;
  fsm->nevents = NUM_EVENTS;
  fsm->transition = parse_transition;

  fsm->runtime = 0;
  fsm->live = true;

  return fsm;
}

// TODO: Complete this table mapping state/events to the target state
static state_t const _transitions[NUM_STATES][NUM_EVENTS] = {
  // ADMIT DISPATCH SCHEDULE BLOCK UNBLOCK EXIT KILL NIL
  { RDY, NST, NST, NST, NST, NST, TRM, NST }, // NEW
  { NST, RUN, NST, BLK, NST, NST, TRM, NST }, // READY
  { NST, RUN, RDY, BLK, NST, TRM, NST, NST }, // RUNNING
  { NST, NST, NST, NST, RDY, NST, TRM, NST }, // BLOCKED
  { NST, NST, NST, NST, NST, NST, NST, NST }  // TERM
};

// TODO: Create a table mapping states/events to the effect functions
static action_t const _effect[NUM_STATES][NUM_EVENTS] = {
  // ADMIT DISPATCH SCHEDULE BLOCK UNBLOCK EXIT KILL NIL
  { set_live, NULL, NULL, NULL, NULL, NULL, kill_proc, NULL },      // NEW
  { NULL, NULL, NULL, NULL, NULL, NULL, kill_proc, NULL },          // READY
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },               // RUNNING
  { NULL, NULL, NULL, NULL, say_unblocked, NULL, kill_proc, NULL }, // BLOCKED
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }                // TERM
};

// TODO: Create an array of action_t to map each state to its entry function
static action_t const _entry[NUM_STATES]
    = { reset_runtime, NULL, incr_runtime, say_blocked, print_stats };

// Transition lookup function. It should perform the transition lookup and
// return the next state after the transition
static state_t
parse_transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{

  // return -1 if the state/event combination is invalid
  if (fsm->state >= NST || event >= NIL
      || _transitions[fsm->state][event] == NST)
    return -1;

  // Look up effect and state/event in the table
  *effect = _effect[fsm->state][event];

  // Look up next state in the entry events list
  state_t next_state = _transitions[fsm->state][event];
  if (next_state != NST)
    *entry = _entry[next_state];

  return next_state;
}