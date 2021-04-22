#pragma once

typedef struct process
{
  int pid;
  char* name;
  int priority;
  char state; // toma los valores 'E' (Running), 'R' (Ready), 'W'(Waiting) y 'F' (Finished)
  struct process* next;
  struct process* prev;
  int turns;
  int interruptions;
  time_t turnaround;
  time_t response;
  time_t wating;
} Process;

void process_init(Process* process, Process* next, int pid, char* name, int priority, char* state);
void interrupt_process(Process* process);
void continue_process(Process* process);
void priority_update(Process* process, int priority);
void finish_process(Process* process, FILE* output_file);