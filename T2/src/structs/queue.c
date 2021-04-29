#include "queue.h"
#include <assert.h> 

/** Saca el q->curr_process de la queue
 * @param q Queue
 * @returns puntero a proceso sacado
 */
Process* collapse_process(Queue *q){
  Process* p = q->curr_process;
  if (q->curr_process == q->end_process)
    q->end_process = q->curr_process->prev;
  if (q->curr_process == q->start_process)
    q->start_process = q->curr_process->next;
  if (q->curr_process->prev){
    q->curr_process->prev->next = q->curr_process->next;
    // dejar como proceso actual el anterior:
  }
  if (q->curr_process->next){
    q->curr_process->next->prev = q->curr_process->prev;
    q->curr_process = q->curr_process->next;
  }
  else  // no hay proceso previo, se asume que curr esta en start
    q->curr_process = q->start_process;  // por lo que el anterior es end
  return p;
}

void add_process(Queue* q, Process* p){
  if (q->end_process)
    q->end_process->next = p;
  p->prev = q->end_process;  // si es NULL no importa
  q->end_process = p;
  p->quantum = q->quantum;
  if (!(q->start_process)){
    q->start_process = p;
    q->curr_process = p;
  }
}

/** Saca el mueve un proceso al final de otra cola
 * @param from Queue in
 * @param to Queue out
 * @returns void
 */
void move_process(Queue *from, Queue *to){
  Process *p = collapse_process(from);
  p->next = NULL;
  if (to->end_process){
    to->end_process->next = p;
    p->prev = to->end_process;
  }
  if (!(to->start_process)){
    to->start_process = p;
    to->curr_process = p;
    p->prev = NULL;
  }
  p->quantum = to->quantum;
  to->end_process = p;
}

void remove_process(Queue *q){
  collapse_process(q);
}

void next_process(Queue *q){
  if (q->curr_process && q->curr_process->next)
    q->curr_process = q->curr_process->next;
  else
    q->curr_process = q->start_process;
}