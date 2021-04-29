#include <stdio.h>
#include <stdlib.h> 
#include "../file_manager/manager.h"
#include "../structs/queue.h"
#include "../structs/process.h"

int time_now=0;
int next_arrival=-1;

int main(int argc, char **argv)
{
  InputFile *file_input = read_file(argv[1]);
  FILE *file_output = fopen(argv[2], "w");
  int Q = atoi(argv[3]), q = atoi(argv[4]), S = atoi(argv[5]);
  Queue *queues = calloc(Q, sizeof(Queue)), *queue_running=NULL;
  for (int i=0; i<Q; i++)
    queues[i].quantum = (Q-(Q-1-i))*q;

  Process *process = malloc(file_input->len * sizeof(Process)),
          *process_running=NULL, *t_process=NULL;
  
  for (int i=0; i<file_input->len; i++){
    process[i] = process_init(file_input->lines[i][0],
                              atoi(file_input->lines[i][1]),
                              atoi(file_input->lines[i][2]),
                              atoi(file_input->lines[i][3]),
                              atoi(file_input->lines[i][4]),
                              atoi(file_input->lines[i][5]));
  }
  // tiempos que van a ciclar
  int S_left=S, queue_number, curr_queue_number;

  // ciclos de programa
  for (;;time_now++, next_arrival=-1){
    printf("\ntime: %d ; S:%d\n", time_now, S_left);
    // 1.-  ver si hay procesos que entran
    for (int i=0; i < file_input->len; i++){
      if (process[i].arrival == time_now){
        // agregar proceso a primera cola
        add_process(&queues[0], &process[i]);
      }
      else if (process[i].arrival > time_now){
        // guardar siguiente arrival (TIEMPO_INICIO)
        if (next_arrival == -1 || next_arrival > process[i].arrival){
          next_arrival = process[i].arrival;
        }
      }
    }
    // 2.- buscar procesos que pasan a ready
    // buscar fin del wait del proceso & sumar tiempos de espera
    for (int i=0; i<file_input->len; i++){
      // primero revisar si el proceso ha llegado
      printf("%s %c %d %d %d %d\n", process[i].name, process[i].state, process[i].quantum, process[i].turns, process[i].waiting, process[i].cycles);
      if (process[i].arrival <= time_now)
        switch (process[i].state)
        {
          case 'W':
            if ((process[i].waiting_init+1) && process[i].waiting_init + process[i].wait_delay == time_now)
              process[i].state='R';
          case 'R':
            process[i].waiting++;
        }
    }
    // .30000000000000004- llegar hasta la cola
    // buscar la primera cola no vacía, si no hay, dar el numero -1
    for (queue_number=0; !(queues[queue_number].start_process); queue_number++){
      if (queue_number+1 >= Q){
        queue_number = -1;
        break;
      }
    }
    // ver si no hay nada en el futuro (no quedan mas procesos)
    if (queue_number == -1 && next_arrival == -1) break;
    // si hay procesos para ejecutar:
    if (queue_number != -1){

      // si no hay programa en running, obtener uno en ready
      if (!process_running){
        for (;queue_number<Q; queue_number++){
          if (!queues[queue_number].start_process) continue;  // si no hay proceso actual, se pasa a la siguiente
          process_running = queues[queue_number].start_process;
          if (process_running->state != 'R'){
            t_process = process_running->next;
            // se hace el for con los otros procesos buscando uno que esté en 'R'
            if (!t_process) process_running=NULL;
            else for(;t_process->state != 'R';)
            {
              t_process=t_process->next;
              if(!t_process){
                process_running = NULL;  // no se encontró un proceso en 'R'
                break;
              }
            }
          }
          if (process_running) break;
        }
        // dejar el proceso en running, y la cola actual
        if (process_running){
          curr_queue_number = queue_number;
          continue_process(process_running, time_now);
          process_running->waiting--;  // evitar contar doble
          queue_running = &queues[queue_number];
        }
      }  // si hay proceso para correr, se corre
      if (process_running){
        queue_running->curr_process = process_running;
        process_running->quantum--;
        process_running->cycles--;
        // si espera alguna vez, no cuento dlble
        if (process_running->wait != -1)
          process_running->wait--;
        if (!process_running->cycles){  // si el proceso terminó
          // si el proceso termina su quantum, se interrumpe tambien (caso borde issue #143)
          if (!process_running->quantum) process_running->interruptions++;
          finish_process(process_running, file_output, time_now+1);  // se suma 1 para que la suma incluya el final
          remove_process(queue_running);
          process_running=NULL;
        }  // si se acaba el quantum, sale, y baja de queue
        else if (!process_running->quantum){
          if (queue_running != &queues[Q-1]){  // pasa a la siguiente queue
            move_process(queue_running, 1+queue_running);
          }else{
            process_running->quantum = queue_running->quantum;
          }
          // si debe ocurrir wait, se hace
          if (!process_running->wait){
            give_cpu_process(process_running, time_now);
          }
          interrupt_process(process_running);
          process_running=NULL;
          
        }  // si el proceso hace wait
        else if (!process_running->wait){
          if (queue_running != &queues[0]){  // pasa a la anterior queue
            move_process(queue_running, -1+queue_running);
          }else{
            process_running->quantum = queue_running->quantum;
          }
          give_cpu_process(process_running, time_now);
          process_running=NULL;
        }
      }
    }

    // avanzar tiempo
    S_left--;
    if (!S_left){
      S_left = S;
      // mover todos menos curr_process
      // si se mueve un ready, se debe reescribir el queue_running
      // iterar por todos los procesos de la queue
      int queue_endings;
      for (queue_number=1; queue_number<Q; queue_number++){
        // no mover el proceso ejecutandose
        queue_endings = 0;
        queues[queue_number].curr_process = queues[queue_number].start_process;
        while(queues[queue_number].start_process && queue_endings != 1){
          if (queues[queue_number].curr_process == queues[queue_number].end_process)
            queue_endings++;
          // mover procesos no en exec al final
          if (queues[queue_number].curr_process->state != 'E'){
            if (queues[queue_number].curr_process == process_running)
              process_running = NULL;
            move_process(&queues[queue_number], &queues[0]);
          }  // avanzar al siguiente proceso
          next_process(&queues[queue_number]);
        }
        queues[queue_number].curr_process = process_running;
      }
      // recuperar process_runningv
      if (process_running){
        queue_running->curr_process = process_running;
      }
    }
  }

  fclose(file_output);
  free(process);
  free(queues);
  input_file_destroy(file_input);
}
