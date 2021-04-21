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

void process_destroy(Process* process)
{
  free(process);
}