#pragma once

typedef struct process
{
  int pid;
  char* name;
  int priority;
  char* state;
} Process;

Process* process_init(int pid, char* name, int priority, char* state);
// void state_update(Process* process, int state);
void process_destroy(Process* process);