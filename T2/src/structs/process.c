#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process process_init(char* name, int pid, time_t arrival, int cycles, time_t wait, time_t wait_delay)
{
  Process process = (Process){
    .pid = pid,
    .name = name,
    .cycles = cycles,
    .wait = (wait != 0 ? wait : -1),  // si nunca espera, se deja en -1 (caso borde si arrival es 0)
    .wait_delay = wait_delay,
    .state = 'R',
    .next = NULL,
    .prev = NULL,
    .turns = 0,
    .interruptions = 0,
    .arrival = arrival,
    .response = 0,
    .waiting = 0,
    .waiting_init = -1,
    .quantum = -1
  };
  return process;
}

void interrupt_process(Process* process)
{
  process -> interruptions++;
  process -> turns++;
  process -> state = 'R';
}

void give_cpu_process(Process* process)
{
  process -> interruptions++;
  process -> state = 'W';
  // priority change
}

void continue_process(Process* process, time_t time_now)
{
  process -> state = 'E';
  if (! process -> turns)
  {
    process -> response = time_now - process -> arrival;
  }
}

void finish_process(Process* process, FILE* output_file, time_t time_now)
{
  process -> state =  'F';
  process -> turns++;
  fprintf(output_file, "%s,%i,%i,%ld,%ld,%ld\n",
    process -> name,
    process -> turns,
    process -> interruptions,
    time_now - process -> arrival,
    process -> response,
    process -> waiting
  );
}