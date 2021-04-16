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

InputFile* data_in;

void sig_int_handler(int);
void sig_abrt_handler(int), create_manager(int, int), create_worker(int);
void sig_alrm_handler(int);

int status, len_args, n_childs;
pid_t pid_main, pid_child;
time_t turnaround;
pid_t *child_pids;

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
  signal(SIGABRT,sig_abrt_handler); // Register signal handler
  signal(SIGINT,sig_int_handler); // Register signal handler
  data_in = read_file(argv[1]);
  pid_main = getpid();
  int id = atoi(argv[2]);

  // encontrar tipo de programa
  ptype_this = char_to_pt(data_in->lines[id][0][0]);
  printf("ptype: %d\n", ptype_this);
  // hacer acciones necesarias
  if (ptype_this == W){
    create_worker(id);
  }else{
    // crear timeout
    create_manager(id, (int)(ptype_this == R));
  }


  printf("filelines: %d\n", data_in->len);
  printf("%d\n", atoi(argv[2]));
  printf("pid: %d\n", pid_main);
}


// https://linuxhint.com/signal_handlers_c_programming_language/
void sig_abrt_handler(int signum){
  // pasa error a los hijos
  printf("abort signal detected\n");
  if (ptype_this == W)
    kill(pid_child, SIGABRT);
  else
    for (int i=0; i<n_childs; i++)
      kill(child_pids[i], SIGABRT);
  
}

void sig_alrm_handler(int signum){
  printf("timeout signal detected\n");
  for (int i=0; i<n_childs; i++)
    kill(pid_child, SIGABRT);
}


void sig_int_handler(int signum){
  if (ptype_this == R){
    printf("interrupt signal detected\n");
    for (int i=0; i<n_childs; i++)
      kill(child_pids[i], SIGABRT);
  }
}

void create_worker(int w_id){
  pid_child = fork();
  len_args = atoi(data_in->lines[w_id][2]);
  if (pid_child == 0){
    signal(SIGINT, SIG_IGN);
    char** command = calloc(len_args + 2, sizeof(char*));
    command[0] = data_in->lines[w_id][1];
    printf("args: %s\n", command[0]);
    for (int i = 0; i < len_args; i++)
    {
      command[i + 1] = data_in->lines[w_id][3+i];
    }
    command[len_args + 1] = (char*)NULL;
    execvp(command[0], command);
  }
  else{
    turnaround = time(NULL);
    waitpid(pid_child, &status, WUNTRACED);
    turnaround = time(NULL) - turnaround;
    int interrupted_child;

    if (WTERMSIG(status) | WSTOPSIG(status)) interrupted_child = 1;
    else interrupted_child = 0;

    char* file_name = malloc(sizeof(char) * (10 + 6));
    sprintf(file_name, "%i.txt", w_id);
    FILE *output_file;
    output_file = fopen(file_name, "w");
    fprintf(output_file, "%s,", data_in->lines[w_id][1]);
    for (int i = 0; i < len_args; i++)
    {
      fprintf(output_file, "%s,", data_in->lines[w_id][3+i]);
    }
    fprintf(output_file, "%ld,%i,%i\n", turnaround, status, interrupted_child);
    fclose(output_file);
    free(file_name);
    input_file_destroy(data_in);
    exit(0);
  }
}

void create_manager(int m_id, int is_root){
  signal(SIGALRM, sig_alrm_handler); // Register signal handler

  // CREATE TIMER
  alarm(atoi(data_in->lines[m_id][1]));
  int t;
  n_childs = atoi(data_in->lines[m_id][2]);
  child_pids = malloc(n_childs*sizeof(pid_t));
  // int *child_status = malloc(n_childs*sizeof(int));
  for (int i=0; i<n_childs; i++){
    t = atoi(data_in->lines[m_id][3+i]);  // id del hijo
    if (char_to_pt(data_in->lines[t][0][0]) == W){  // trabajador
      pid_child = fork();
      if (pid_child == 0){
        ptype_this = W;
        free(child_pids);
        create_worker(t);
        exit(0);
      }
    }
    else {  // manager
      pid_child = fork();
      if (pid_child == 0){
        ptype_this = M;
        free(child_pids);
        create_manager(t, 0);
        exit(0);
      }
    }
    child_pids[i] = pid_child;
  }
  // hacer wait
  for (int i=0; i<n_childs; i++){
    wait(NULL);
  }
  char* file_out_name = malloc(sizeof(char) * (10 + 6));
  sprintf(file_out_name, "%i.txt", m_id);
  char c;
  FILE *file_out;
  file_out = fopen(file_out_name, "w");

  for (int i=0; i<n_childs; i++){
    char* file_name = malloc(sizeof(char) * (10 + 6));
    sprintf(file_name, "%i.txt", atoi(data_in->lines[m_id][3+i]));
    FILE* file_in;
    file_in = fopen(file_name, "r");
    
    while((c = fgetc(file_in)) != EOF){
      fputc(c, file_out);
    }
    fclose(file_in);
    free(file_name);
  }
  fclose(file_out);
  free(file_out_name);
  free(child_pids);
  
  input_file_destroy(data_in);
  exit(0);
}
