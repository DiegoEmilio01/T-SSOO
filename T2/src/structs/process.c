#include <stdio.h>
#include <stdlib.h>
#include "process.h"

void process_init(Process* process, Process* next, int pid, char* name, int priority, char* state)
{
  process -> pid = pid;
  process -> name = name;
  process -> priority = priority;
  process -> state = state;
  process -> next = next;
  process -> prev = NULL;
  process -> turns = 0;
  process -> interruptions = 0;
  process -> turnaround = 0;
  process -> response = 0;
  process -> wating = 0;
}

void interrupt_process(Process* process)
{
  process -> interruptions++;
  process -> turns++;
  process -> state = 'R';
}

void continue_process(Process* process)
{
  process -> state = 'E';
}


void priority_update(Process* process, int priority)
{
  process -> priority = priority;
}

void finish_process(Process* process, FILE* output_file)
{
  process -> state =  'F';
  process -> turns++;
  FILE *output_file = fopen(output_file, "w");
  fprintf(output_file, "%s,%i,%i,%ld,%ld,%ld\n",
    process -> name,
    process -> turns,
    process -> interruptions,
    process -> turnaround,
    process -> response,
    process -> wating
  );
}