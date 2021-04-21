#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process* process_init(int pid, char* name, int priority, char* state)
{
  Process* process = malloc(sizeof(Process));
  *process = (Process){
    .pid = pid,
    .name = name,
    .priority = priority,
    .state = state,
  };
}

void priority_update(Process* process, int priority)
{
  process -> priority = priority;
}

void state_update(Process* process, char* state)
{
  process -> state = state;
}

void next_update(Process* process, Process* next)
{
  process -> next = next;
}

void process_destroy(Process* process)
{
  free(process);
}