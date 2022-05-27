const int MAX_PROCESS_NUMBER = 2048;

const int min_granularity = 2;
const int schedule_latency = 12;
static const int nice_to_weight[40] = {
/* -20 */ 88761, 71755, 56483, 46273, 36291,
/* -15 */ 29154, 23254, 18705, 14949, 11916,
/* -10 */ 9548, 7620, 6100, 4904, 3906,
/* -5 */ 3121, 2501, 1991, 1586, 1277,
/* 0 */ 1024, 820, 655, 526, 423,
/* 5 */ 335, 272, 215, 172, 137,
/* 10 */ 110, 87, 70, 56, 45,
/* 15 */ 36, 29, 23, 18, 15,
};
const int default_weight = 1024;

struct PROCESS {
  int id, nice, length, weight, vruntime, runtime;
} typedef process;

struct HEAP {
  process **array;
  int size;
} typedef heap;

heap *make_heap(process **p, int n);
void heap_destroy(heap *h);
void heapify(heap *h, int i);
void insert(heap *h, process *x);
process* extract_min(heap *h);

void get_schedule(process **p, int n);