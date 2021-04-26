#include <stdio.h>
#include "../file_manager/manager.h"
#include "../structs/queue.h"
#include "../structs/process.h"

int time_now=-1;
int next_arrival=-1;

int main(int argc, char **argv)
{
  InputFile *file_input = read_file(argv[1]);
  FILE *file_output = fopen(argv[2], "w");
  int Q = atoi(argv[3]), q = atoi(argv[4]), S = atoi(argv[5]);
  Queue *queues = calloc(Q, sizeof(Queue)), *queue_running=NULL;
  for (int i=0; i<Q; i++)
    queues[i].quantum = (1+i)*q;

  Process *process = malloc(file_input->len * sizeof(Process)),
          *process_running=NULL;
  
  for (int i=0; i<file_input->len; i++){
    process[i] = process_init(file_input->lines[i][0],
                              atoi(file_input->lines[i][1]),
                              atoi(file_input->lines[i][2]),
                              atoi(file_input->lines[i][3]),
                              atoi(file_input->lines[i][4]),
                              atoi(file_input->lines[i][5]));
  }
  // tiempos que van a ciclar
  int S_left=S, queue_number;

  // ciclos de programa
  for (;;time_now++, next_arrival=-1){
    //1.-  ver si hay procesos que entran
    for (int i=0; i < file_input->len; i++){
      if (process[i].arrival == time_now){
        // agregar proceso a primera cola
        add_process(&queues[0], &process[i]);
        queue_running=&queues[0];
      }
      else if (process[i].arrival > time_now){
        // guardar siguiente arrival (TIEMPO_INICIO)
        if (next_arrival == -1 || next_arrival > process[i].arrival){
          next_arrival = process[i].arrival;
        }
      }      
    }
    // 2.- llegar hasta la cola
    // buscar la primera cola no vacía, si no hay, dar el numero -1
    for (queue_number=0; !(queues[queue_number].start_process); queue_number++)
      if (queue_number+1 >= file_input->len){
        queue_number = -1;
        break;
      }
    // ver si no hay nada en el futuro (no quedan mas procesos)
    if (queue_number == -1 && next_arrival == -1) break;
    // si hay procesos para ejecutar:
    if (queue_number != -1){

      // si no hay programa en running, obtener uno en ready
      if (!process_running){
        for (;queue_number<file_input->len; queue_number++){
          if (!queues[queue_number].curr_process) continue;  // si no hay proceso actual, se pasa a la siguiente
          process_running = queues[queue_number].curr_process;
          if (process_running->state != 'R'){
            next_process(&queues[queue_number]);
            // se hace el for con los otros procesos buscando uno que esté en 'R'
            if (queues[queue_number].curr_process == process_running) process_running=NULL;
            else for(Process *curr_process=queues[queue_number].curr_process; curr_process->state!='R'; next_process(&queues[queue_number]))
            {
              if(curr_process == process_running){
                process_running = NULL;  // no se encontró un proceso en 'R'
                break;
              }
            }
          }
          if (process_running) break;
        }
        // dejar el proceso en running, y la cola actual
        if (process_running){
          continue_process(process_running, time_now);
          queue_running = &queues[queue_number];
        }
        
      }  // si hay proceso para correr, se corre
      if (process_running){
        process_running->quantum--;
        process_running->cycles--;
        if (!process_running->cycles){  // si el proceso terminó
          // si el proceso termina su quantum, se interrumpe tambien (caso borde issue #143)
          if (!process_running->quantum) process_running->interruptions -=- 1;
          finish_process(process_running, file_output, time_now);
          remove_process(process_running);
          process_running=NULL;
        }  // si se acaba el quantum, sale, y baja de queue
        else if (!process_running->quantum)){

        }
      }

    }
    // avanzar tiempo
    S_left--;
    if (!S_left){
      S_left = 0;
      // mover todos menos curr_process
      // si se mueve un ready, se debe reescribir el queue_running
    }
  }








  
  free(process);
  free(queues);
  input_file_destroy(file_input);
}
