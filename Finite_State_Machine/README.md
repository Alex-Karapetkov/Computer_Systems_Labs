# Lab: Process Lifecycle Finite State Machine Simulator

This lab implements a **finite state machine (FSM)** in C that simulates the lifecycle of a process within an operating system. The FSM models key process states and transitions triggered by system events.

## Lab Overview

The FSM represents a simplified model of a process lifecycle, tracking the states a process moves through during its execution:

- **NEW**: Process created but not yet admitted.
- **READY (RDY)**: Process is ready to run.
- **RUNNING (RUN)**: Process is currently executing.
- **BLOCKED (BLK)**: Process is blocked, waiting for an event.
- **TERMINATED (TRM)**: Process has ended and can no longer transition.

Transitions between states are driven by events such as:

- `ADMIT` — admit a process into the system.
- `DISPATCH` — dispatch a process to run.
- `SCHEDULE` — move a running process back to the ready queue.
- `BLOCK` — block the running process.
- `UNBLOCK` — unblock a blocked process.
- `EXIT` — process exits voluntarily.
- `KILL` — system kills a process.
- `NIL` — no operation event.

The FSM uses tables of transitions, effect functions (side effects), and entry actions to handle state changes robustly.

---

## Background

This lab is based on material from **Michael S. Kirkpatrick’s Operating Systems** textbook (Section 1.7), which provides foundational concepts and templates for FSM design and process lifecycle modeling.

The code and implementation here are **my own work**. This repository is provided for viewing purposes only and **may not be used or modified by others**.

---

## Running the Program

The executable simulates the FSM by accepting a sequence of event codes as command-line arguments. Each event triggers a transition and associated effects within the FSM.

### Event Codes:

| Event    | Code |
|----------|------|
| ADMIT    | 0    |
| DISPATCH | 1    |
| SCHEDULE | 2    |
| BLOCK    | 3    |
| UNBLOCK  | 4    |
| EXIT     | 5    |
| KILL     | 6    |
| NIL      | 7    |

### Usage:

```bash
./fsm [event_code1 event_code2 ...]
```
Example:
```bash
./fsm 0 1 3 4 6
```
This runs the FSM with the event sequence: ADMIT, DISPATCH, BLOCK, UNBLOCK, and KILL.
The program prints the state transitions and relevant messages and expects the process to end in the TERMINATED (TRM) state for a successful simulation.

