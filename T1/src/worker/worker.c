#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>  // https://pubs.opengroup.org/onlinepubs/9699919799/toc.htm
#include "worker.h"


pid_t childpid = -1;
char* name;
int n_arguments;
char** arguments;
pid_t id;
int status;
time_t turnaround;


int main(int argc, char **argv)
{
  char* arguments[] = {"1", "2"};
  worker_exec("./sum", 2, arguments, 4);
}

void worker_exec(char* _name, int _n_arguments, char** _arguments, pid_t _id)
{
  signal(SIGINT, sig_int_handler);    // Register signal handler
  signal(SIGABRT, sig_abrt_handler);  // Register signal handler
  name = _name;
  n_arguments = _n_arguments;
  arguments = _arguments;
  id = _id;
  
  childpid = fork();
  time(&turnaround);
  if (childpid >= 0)
  {
    if (childpid == 0)
    {
      signal(SIGINT, SIG_IGN);
      char** command = calloc(n_arguments + 2, sizeof(char*));
      command[0] = name;
      for (int i = 0; i < n_arguments; i++)
      {
        command[i + 1] = arguments[i];
      }
      command[n_arguments + 1] = (char*)NULL;
      execvp(name, command);
    }
    else
    {
      worker_tasks(0);
      exit(0);
    }
  }
  else
  {
    perror("fork");
    exit(0);
  }
}

// https://linuxhint.com/signal_handlers_c_programming_language/
void sig_abrt_handler(int signum){
  printf("Timeout or abort signal detected\n");
  if (childpid > 0){
    kill(childpid, SIGABRT);
    worker_tasks(1);
  }
  exit(0);
}

void sig_int_handler(int signum){
  printf("\nInterrupt signal detected\n");
  if (childpid > 0){
    worker_tasks(0);
  }
  exit(0);
}

void worker_tasks(int interrupted)
{
  pid_t wpid = waitpid(childpid, &status, WUNTRACED);
  turnaround = time(NULL) - turnaround;

  if (WTERMSIG(status) | WSTOPSIG(status)) printf("Bad ending\n");
  else printf("Good ending\n");

  char* file_name = malloc(sizeof(char) * (10 + 6));
  sprintf(file_name, "%i.txt", id);
  FILE *output_file = fopen(file_name, "w");
  fprintf(output_file, "%s,", name);
  for (int i = 0; i < n_arguments; i++)
  {
    fprintf(output_file, "%s,", arguments[i]);
  }
  fprintf(output_file, "%ld,%i,%i\n", turnaround, status, 0);
  fclose(output_file);
  free(file_name);
}