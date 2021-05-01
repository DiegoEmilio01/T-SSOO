#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process process_init(char* name, int pid, int arrival, int cycles, int wait, int wait_delay)
{
  Process process = (Process){
    .pid = pid,
    .name = name,
    .cycles = cycles,
    .wait = (wait != 0 ? wait : -1),  // si nunca espera, se deja en -1 (caso borde si arrival es 0)
    .orig_wait = (wait != 0 ? wait : -1),
    .wait_delay = wait_delay+1,
    .state = 'R',
    .next = NULL,
    .prev = NULL,
    .turns = 0,
    .interruptions = 0,
    .arrival = arrival,
    .response = 0,
    .waiting = 0,
    .waiting_init = (int)-1,
    .quantum = -1
  };
  return process;
}

void interrupt_process(Process* process)
{
  process -> interruptions++;
  process -> state = 'R';
}

void give_cpu_process(Process* process, int time_now)
{
  process -> state = 'W';
  process->waiting_init = time_now;
}

void continue_process(Process* process, int time_now)
{
  process -> state = 'E';
  if (! process -> turns)
  {
    process -> response = time_now - process -> arrival;
  }
  process -> turns++;
}

void finish_process(Process* process, FILE* output_file, int time_now)
{
  process -> state =  'F';
  fprintf(output_file, "%s,%i,%i,%d,%d,%d\n",
    process -> name,
    process -> turns,
    process -> interruptions,
    time_now - process -> arrival,
    process -> response,
    process -> waiting
  );
}