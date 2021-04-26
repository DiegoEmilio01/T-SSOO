#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process process_init(char* name, int pid, time_t arrival, int cycles, time_t wait, time_t wait_delay)
{
  Process process = (Process){
    .pid = pid,
    .name = name,
    .cycles = cycles,
    .wait = wait,
    .wait_delay = wait_delay,
    .state = 'A',
    .next = NULL,
    .prev = NULL,
    .turns = 0,
    .interruptions = 0,
    .arrival = arrival,
    .response = 0,
    .waiting = 0,
    .waiting_init = -1
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

void continue_process(Process* process, time_t response)
{
  process -> state = 'E';
  if (! process -> turns)
  {
    process -> response = response;
  }
}

void finish_process(Process* process, FILE* output_file, time_t finishing_time, char* name)
{
  process -> state =  'F';
  process -> turns++;
  FILE *output_file = fopen(output_file, "w");
  fprintf(output_file, "%s,%i,%i,%ld,%ld,%ld\n",
    name,
    process -> turns,
    process -> interruptions,
    finishing_time - process -> arrival,
    process -> response,
    process -> waiting
  );
}