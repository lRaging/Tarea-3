#ifndef Heap_h
#define Heap_h

typedef struct Heap Heap;

Heap* createHeap();
void* heap_top(Heap* pq);
void heap_push(Heap* pq, void* data, int priority);
void heap_pop(Heap* pq);
bool isEmptyHeap(Heap *pq);
Heap* copyHeap(const Heap* pq);
void heap_remove(Heap* pq, void* data, int priority);
#endif 