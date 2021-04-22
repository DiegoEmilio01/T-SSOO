// Tells the compiler to compile this file once
#pragma once

#include "process.h"

typedef struct queue {
  int quantum;  // quantum value
  Process* curr_process;
  Process* start_process;
  Process* end_process;
} Queue;

Process* add_process(Queue*);

void move_process(Queue*, Queue*);

void remove_process(Queue*);

void next_process(Queue*);