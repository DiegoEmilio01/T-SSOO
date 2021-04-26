#include <stdio.h>
#include <time.h>

#pragma once

typedef struct process
{
  int pid;
  char* name;
  int cycles;
  time_t wait;            // N de wait
  time_t wait_delay;      // El delay de cada wait
  char state;             // toma los valores 'E' (Running), 'R' (Ready), 'W'(Waiting), 'F' (Finished) y 'A' (Arrival).
  struct process* next;
  struct process* prev;
  int turns;
  int interruptions;
  time_t arrival;         // tiempo (timestamp) en que entra al sistema
  time_t response;        // tiempo que toma desde que entrea hasta el primer E
  time_t waiting;         // tiempo que existe pero no corre (en R o W)
  time_t waiting_init;    // tiempo (timestamp) en que entro a wait
  int quantum;
} Process;

Process process_init(char* name, int pid, time_t arrival, int cycles, time_t wait, time_t wait_delay);
void interrupt_process(Process* process);
void continue_process(Process* process, time_t total_time);
void finish_process(Process* process, FILE* output_file, time_t total_time, char* name);