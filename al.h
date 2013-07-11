#define MIN_SIZE 10
#define ADD_THRESHOLD 1
#define ADD_SIZE_MULTIPLY_FACTOR 2
#define DEL_THRESHOLD 3
#define DEL_SIZE_DIVIDE_FACTOR 2

typedef struct ArrayList
{
	void **array;
	unsigned long size;
	unsigned long memory_size;
	int (*compareFn)(void*, void*);
	void (*freeFn)(void*);
	void (*printFn)(void*);
} AL;

AL* al_create(unsigned int size);
void* al_get(AL *list, unsigned long index);
void* al_set(AL *list, unsigned long index, void *data);
unsigned long al_push(AL *list, void *data);
void* al_pop(AL *list);
unsigned long al_push(AL *list, void *data);
unsigned long al_add(AL *list, unsigned long index, void *data);
void* al_del(AL *list, unsigned long index);
void al_delRange(AL *list, unsigned long start, unsigned long end);
void al_addAll(AL *list, unsigned int data_size, void **data);
void al_reverse(AL *list);
void al_clear(AL *list);
void al_print(AL *list);