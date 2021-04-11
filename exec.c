#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <time.h>

#include <sys/wait.h>  // https://pubs.opengroup.org/onlinepubs/9699919799/toc.htm

#include <signal.h>  // https://stackoverflow.com/questions/5867714/how-to-handle-sigabrt-signal-in-unix


void sig_int_handler(int);
void sig_abrt_handler(int);


pid_t child_pid = -1;

int main(int argc, char **argv)
{
  signal(SIGINT,sig_int_handler); // Register signal handler
  signal(SIGABRT,sig_abrt_handler); // Register signal handler


  // creat timer
  if (fork()==0){
    sleep(3);
    kill(getppid(),SIGABRT);
    exit(0);
  }
  child_pid = fork();
  int status;
  time_t turnaround = time(NULL);
  if (child_pid==0){
    // char* command[] = {"./sum", "3", "4", (char*)NULL};
    // execvp("./sum", command);
    int b[] = {0, 1,2,3};
    int a;
    scanf("%d", &a);
    printf("%d\n", b[a]);

  }else{
    pid_t wpid = waitpid(child_pid, &status, WUNTRACED);
    printf("exec_time: %ld\n", time(NULL) - turnaround);

    if (WTERMSIG(status) | WSTOPSIG(status)) printf("bad end\n");
    else printf("normal end\n");
  }
  return 0;
}

// https://linuxhint.com/signal_handlers_c_programming_language/
void sig_abrt_handler(int signum){
  printf("timeout or abort signal detected\n");
  if (child_pid != 0){
    kill(child_pid,SIGABRT);
    pid_t wpid = waitpid(child_pid, NULL, WUNTRACED);
  }
  exit(0);
}


void sig_int_handler(int signum){
  printf("interrupt signal detected\n");
  exit(0);
}

