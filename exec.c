#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <time.h>

#include <sys/wait.h>  // https://pubs.opengroup.org/onlinepubs/9699919799/toc.htm

int main(int argc, char **argv)
{
  pid_t child_pid = fork();
  int status;
  time_t turnaround = time(NULL);
  if (child_pid==0){
    // char* command[] = {"sum", "3", "4", (char*)NULL};
    // execvp("./sum", command);
    int b[] = {0, 1,2,3};
    int a;
    scanf("%d", &a);
    printf("%d\n", b[a]);

  }else{
    pid_t wpid = waitpid(child_pid, &status, WUNTRACED);
    printf("exec_time: %ld\n", time(NULL) - turnaround);
    // if (wpid == -1) {
    //     perror("waitpid");
    //     exit(EXIT_FAILURE);
    // }

    if (WTERMSIG(status) | WSTOPSIG(status)) printf("bad end\n");
    else printf("normal end\n");
  }
  return 0;
}