#pragma once

typedef struct process
{
  int pid;
  char* name;
  int priority;
  char* state;
  struct process* next;
  struct process* prev;
  int turns;
  int interruptions;
  time_t turnaround;
  time_t response;
  time_t wating;
} Process;

void process_init(Process* process, int pid, char* name, int priority, char* state);
void interrupt_process(Process* process);
void priority_update(Process* process, int priority);
void state_update(Process* process, char* state);
void next_update(Process* process, Process* next);