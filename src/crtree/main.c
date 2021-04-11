#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file_manager/manager.h"

#include <unistd.h>

#include <time.h>

#include <sys/wait.h>  // https://pubs.opengroup.org/onlinepubs/9699919799/toc.htm

#include <signal.h>  // https://stackoverflow.com/questions/5867714/how-to-handle-sigabrt-signal-in-unix


enum program_type{R,M,W};
enum program_type ptype_this=W;

int *pid_childs;

InputFile* data_in;

FILE *output_file;

void sig_int_handler(int);
void sig_abrt_handler(int);

int pid_main, pid_child, id, status, len_args, interrupted_child=0;
time_t turnaround;

enum program_type char_to_pt(char c){
  switch(c){
    case 'W':
      return W;
    case 'R':
      return R;
    case 'M':
      return M;
    default: 
      return W;
  }
}


int main(int argc, char **argv)
{
  signal(SIGINT,sig_int_handler); // Register signal handler
  signal(SIGABRT,sig_abrt_handler); // Register signal handler
  data_in = read_file(argv[1]);
  pid_main = getpid();
  id = atoi(argv[2]);

  // encontrar tipo de programa
  ptype_this = char_to_pt(data_in->lines[id][0][0]);
  printf("ptype: %d\n", ptype_this);
  // hacer acciones necesarias
  if (ptype_this == W){
    pid_child = fork();
    len_args = atoi(data_in->lines[id][2]);
    if (pid_child == 0){
      signal(SIGINT, SIG_IGN);
      char** command = calloc(len_args + 2, sizeof(char*));
      command[0] = data_in->lines[id][1];
      printf("args: %s\n", command[0]);
      for (int i = 0; i < len_args; i++)
      {
        command[i + 1] = data_in->lines[id][3+i];
      }
      command[len_args + 1] = (char*)NULL;
      execvp(command[0], command);
    }
    else{
      turnaround = time(NULL);
      pid_t wpid = waitpid(pid_child, &status, WUNTRACED);
      turnaround = time(NULL) - turnaround;

      if (WTERMSIG(status) | WSTOPSIG(status)) printf("Bad ending\n");
      else printf("Good ending\n");

      char* file_name = malloc(sizeof(char) * (10 + 6));
      sprintf(file_name, "%i.txt", id);
      output_file = fopen(file_name, "w");
      fprintf(output_file, "%s,", data_in->lines[id][1]);
      for (int i = 0; i < len_args; i++)
      {
        fprintf(output_file, "%s,", data_in->lines[id][3+i]);
      }
      fprintf(output_file, "%ld,%i,%i\n", turnaround, status, interrupted_child);
      fclose(output_file);
      free(file_name);
      input_file_destroy(data_in);
      exit(0);
    }
    
  }else{
    // crear timeout
  }


  printf("filelines: %d\n", data_in->len);
  printf("%d\n", atoi(argv[2]));
  printf("pid: %d\n", pid_main);
}


// https://linuxhint.com/signal_handlers_c_programming_language/
void sig_abrt_handler(int signum){
  // pasa error a los hijos
  printf("timeout or abort signal detected\n");
  if (pid_child != 0){
    interrupted_child = 1;
    kill(pid_child,SIGABRT);
  }
}


void sig_int_handler(int signum){
  printf("interrupt signal detected\n");
  if (ptype_this == R){
    // pasa error a los hijos como sigabort
  }
  else{
    kill(pid_main,SIGABRT);
  }
}
