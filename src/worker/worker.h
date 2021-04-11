#pragma once

#include <time.h>

void worker_exec(char* name, int n_arguments, char** arguments, int id);
void sig_int_handler(int);
void sig_abrt_handler(int);
void worker_tasks(int);