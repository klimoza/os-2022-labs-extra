#include <stdlib.h>
#include <stdio.h>
#include "cfs.h"

int min(int a, int b) {
  if(a < b) return a;
  return b;
}

int max(int a, int b) {
  if(a > b) return a;
  return b;
}

int parent(int i) {
  return (i - 1) / 2;
}

heap *make_heap(process **p, int n) {
  heap *h = (heap*) malloc(sizeof(struct HEAP));
  
  h->size = n;
  h->array = malloc(MAX_PROCESS_NUMBER * sizeof(process*));

  for(int i = 0; i < n; i++)
    h->array[i] = p[i];
  for(int i = n / 2; i >= 0; i--) {
    heapify(h, i);
  }
  
  return h;
}

void heap_destroy(heap *h) {
  free(h->array);
  free(h);
}

void heapify(heap *h, int i) {
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  int min = i;

  if(left < h->size && h->array[left]->vruntime < h->array[min]->vruntime)
    min = left;
  if(right < h->size && h->array[right]->vruntime < h->array[min]->vruntime)
    min = right;
  if(min != i) {
    process *tmp = h->array[i]; 
    h->array[i] = h->array[min];
    h->array[min] = tmp;
    heapify(h, min);
  }
}

void insert(heap *h, process *x) {
  h->array[h->size] = x;
  h->size++;

  int i = h->size - 1;
  while(i != 0 && h->array[parent(i)]->vruntime > h->array[i]->vruntime) {
    process *tmp = h->array[i];
    h->array[i] = h->array[parent(i)];
    h->array[parent(i)] = tmp;
    i = parent(i);
  }
}

process* extract_min(heap *h) {
  process* min = h->array[0];

  h->array[0] = h->array[h->size - 1];
  h->size--;

  heapify(h, 0);
  return min;
}

void print_schedule(process **p, int n) {
  heap *h = make_heap(p, n);

  int number_of_uncomplete_jobs = 0;
  long long total_sum = 0;
  for(int i = 0; i < n; i++) {
    total_sum += p[i]->weight;
    number_of_uncomplete_jobs += (p[i]->length > 0);
  }

  // Print initial state here
  printf("Here is the job list, with the run time and priority of each job:\n");
  for(int i = 0; i < n; i++) {
    printf("Job %d (priority = %d, length = %d)\n", p[i]->id, p[i]->nice, p[i]->length);
  }
  int current_time = 0;
  printf("\nExecution trace: \n");
  while(number_of_uncomplete_jobs > 0) {
    process *current_job = extract_min(h); // take processs with minimal vruntime

    // use formula and check that job doesn't run more than its total length
    int time_slice = current_job->weight * 1ll * schedule_latency / total_sum;
    time_slice = min(max(time_slice, min_granularity), current_job->length - current_job->runtime);

    // run job for `time_slice` seconds
    current_job->runtime += time_slice;
    printf("[time %d] Run job %d for %d secs", current_time, current_job->id, time_slice);
    current_time += time_slice;

    // check if job is finished
    if(current_job->runtime == current_job->length) {
      number_of_uncomplete_jobs--;
      printf(" (DONE)\n");
    } else {
      current_job->vruntime += default_weight * 1ll * current_job->runtime / current_job->weight;
      insert(h, current_job);
      printf("\n");
    }
  }
  heap_destroy(h);
}

/*
  Please enter jobs using following format:
  n
  id_1 nice_1 length_1
  id_2 nice_2 length_2
        ...
  id_n nice_n length_n
*/
int main() {
  int n;
  scanf("%d", &n);
  process **p = malloc(n * sizeof(process*));
  for(int i = 0; i < n; i++) {
    p[i] = malloc(sizeof(process));
    scanf("%d %d %d", &(p[i]->id), &(p[i]->nice), &(p[i]->length));
    p[i]->weight = nice_to_weight[20 + p[i]->nice];
    p[i]->vruntime = 0;
    p[i]->runtime = 0;
  }
  print_schedule(p, n);
  return 0;
}