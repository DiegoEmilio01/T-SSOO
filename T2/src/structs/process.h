#include <stdio.h>
#include <time.h>

#pragma once

typedef struct process
{
  int pid;
  char* name;
  int cycles;
  int wait;            // N de wait
  int wait_delay;      // El delay de cada wait
  char state;             // toma los valores 'E' (Running), 'R' (Ready), 'W'(Waiting) y 'F' (Finished).
  struct process* next;
  struct process* prev;
  int turns;
  int interruptions;
  int arrival;         // tiempo (timestamp) en que entra al sistema
  int response;        // tiempo que toma desde que entrea hasta el primer E
  int waiting;         // tiempo que existe pero no corre (en R o W)
  int waiting_init;    // tiempo (timestamp) en que entro a wait
  int quantum;
} Process;

Process process_init(char* name, int pid, int arrival, int cycles, int wait, int wait_delay);
void interrupt_process(Process* process);
void continue_process(Process* process, int total_time);
void finish_process(Process* process, FILE* output_file, int total_time);
void give_cpu_process(Process*, int);