#pragma once

typedef struct process
{
  int pid;
  char* name;
  int priority;
  char* state;
  struct process* next;
} Process;

Process* process_init(int pid, char* name, int priority, char* state);
void priority_update(Process* process, int priority);
void state_update(Process* process, char* state);
void next_update(Process* process, Process* next);
void process_destroy(Process* process);