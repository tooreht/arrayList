#include <stdio.h>
#include <stdlib.h>
/* uncomment to ignore the assertions (no debug) */
// #define NDEBUG
#include <assert.h>

#include "al.h"

static void increase(AL *list, unsigned long start, unsigned long size, void** data);
static void decrease(AL *list, unsigned long start, unsigned long end);
static void increaseOne(AL *list, unsigned long index, void *data);
static void decreaseOne(AL *list, unsigned long index);


void increase(AL *list, unsigned long start, unsigned long size, void** data)
{
	assert(ADD_THRESHOLD >= 1);

	if(start > list->size)
		start = list->size;

	unsigned long n = start+size;
	if(ADD_THRESHOLD * (list->size+size) >= list->memory_size)
	{
		list->memory_size = list->size * ADD_SIZE_MULTIPLY_FACTOR;
		void **new = malloc(sizeof(void *) * list->memory_size);

		unsigned long i;
		for(i = 0; i < start; i++)
		{
			new[i] = list->array[i];
		}
		for(i = 0; i < size; i++)
		{
			new[i+start] = data[i];
		}
		for(i = start; i <= list->size; i++) // or list->size+1
		{
			new[i+n] = list->array[i];
		}
		free(list->array);
		list->array = new;
	} else {
		unsigned long i;
		for(i = list->size; i > start; i--)
		{
			list->array[i] = list->array[i-1];
		}
		for(i = 0; i < size; i++)
		{
			list->array[i+start] = data[i];
		}
	}
	list->size += size;

	free(data);
	data = NULL;
}

void decrease(AL *list, unsigned long start, unsigned long end)
{
	assert(list);
	assert(start <= end);
	assert(end < list->size);

	unsigned long range = end-start+1;
	unsigned long i;
	for(i = start; i <= end; i++)
	{
		list->freeFn(list->array[i]);
	}

	if(DEL_THRESHOLD * (list->size-range) <= list->memory_size && list->size >= MIN_SIZE)
	{
		list->memory_size /= DEL_SIZE_DIVIDE_FACTOR;
		void **new = malloc(sizeof(void *) * list->memory_size);

		unsigned long i;
		for(i = 0; i < start; i++)
		{
			new[i] = list->array[i];
		}
		for(i = start; i < list->size; i++)
		{
			new[i] = list->array[i+range];
		}

		free(list->array);
		list->array = new;
	} else {
		for(i = start; i < list->size; i++)
		{
			list->array[i] = list->array[i+range];
		}
	}
	list->size -= range;
}

void increaseOne(AL *list, unsigned long index, void *data)
{
	assert(ADD_THRESHOLD >= 1);

	if(index > list->size)
		index = list->size;

	if(ADD_THRESHOLD * list->size >= list->memory_size)
	{
		list->memory_size = list->size * ADD_SIZE_MULTIPLY_FACTOR;
		void **new = malloc(sizeof(void *) * list->memory_size);

		unsigned long i;
		for(i = 0; i < index; i++)
		{
			new[i] = list->array[i];
		}
		new[index] = data;
		for(i = index+1; i <= list->size; i++) // or list->size+1
		{
			new[i] = list->array[i-1];
		}
		free(list->array);
		list->array = new;
	} else {
		unsigned long i;
		for(i = list->size; i > index; i--)
		{
			list->array[i] = list->array[i-1];
		}
		list->array[index] = data;
	}
	list->size++;
}

void decreaseOne(AL *list, unsigned long index)
{
	if(DEL_THRESHOLD * list->size <= list->memory_size && list->size >= MIN_SIZE)
	{
		list->memory_size /= DEL_SIZE_DIVIDE_FACTOR;
		void **new = malloc(sizeof(void *) * list->memory_size);

		unsigned long i;
		for(i = 0; i < index; i++)
		{
			new[i] = list->array[i];
		}
		list->freeFn(list->array[index]);
		// list->size--;
		for(i = index+1; i <= list->size; i++)
		{
			new[i-1] = list->array[i];
		}

		free(list->array);
		list->array = new;
	} else {
		list->freeFn(list->array[index]);
		unsigned long i;
		for(i = index; i < list->size; i++)
		{
			list->array[i] = list->array[i+1];
		}
		//list->array[i] = NULL;
	}
	list->size--;
}

/*
 * Create an array list.
 *
 * @param unsigned int initial size of the array list
 *
 * @return AL pointer to the created array list
 */
AL* al_create(unsigned int size)
{
	assert(size > 0);
	
	AL *new = malloc(sizeof(AL));
	void **array = malloc(sizeof(void *) * size);

	if(new && array)
	{
		new->array = array;
		new->size = 0;
		new->memory_size = size;
	}
	else
	{
		puts("ERROR: Out of memory");
	}

	return new;
}

/*
 * Access an element of the array list.
 *
 * @param AL pointer to the array list
 * @param unsigned long index
 *
 * @return void pointer to the element or NULL if it doesn't exist
 */
void* al_get(AL *list, unsigned long index)
{
	assert(list);

	if(index >= list->size)
		return NULL;
	else
		return list->array[index];
}

/*
 * Access an element of the array list.
 *
 * @param AL pointer to the array list
 * @param unsigned long index
 *
 * @return void pointer to the element or NULL if it doesn't exist
 */
void** al_range(AL *list, unsigned long start, unsigned long end)
{
	assert(list);
	assert(start < end);

	unsigned long size = end - start;
	void **range = malloc(sizeof(void *) * size);

	unsigned long i;
	for(i = 0; start < end; i++)
	{
		range[i] = list->array[i+start++];
	}

	return range;
}

/*
 * Change an element in the array list.
 *
 * @param AL pointer to the array list
 * @param unsigned long index
 * @param void pointer with the new data
 *
 * @return void pointer to the removed data if successfully changed or NULL if it doesn't exist
 */
void* al_set(AL *list, unsigned long index, void *data)
{
	assert(list);

	if(index >= list->size)
		return NULL;
	void* old = list->array[index];
	list->array[index] = data;
	return old;
}

/*
 * Push an element to the array list.
 *
 * @param AL pointer to the array list
 * @param void pointer to the data
 *
 * @return unsigned long index of the inserted element
 */
unsigned long al_push(AL *list, void *data)
{
	assert(list);
	assert(data);
	assert(list->array);

	increaseOne(list, list->size, data);

	return list->size;
}

/*
 * Pop an element from the array list.
 *
 * @param AL pointer to the array list
 * @param void pointer to the data
 *
 * @return void pointer to the removed data or NULL if it doesn't exist
 */
void* al_pop(AL *list)
{
	assert(list);

	if(list->size > 0)
	{
		unsigned long last = list->size-1;
		void* old = list->array[last];

		decreaseOne(list, last);
		return old;
	}
	else
	{
		return NULL;
	}
}

/*
 * Add an element to the array list.
 *
 * @param AL pointer to the array list
 * @param unsingend long index
 * @param void pointer to the data
 *
 * @return unsigned long index of the inserted element
 */
unsigned long al_add(AL *list, unsigned long index, void *data)
{
	assert(list);
	assert(data);
	assert(list->array);

	increaseOne(list, index, data);

	return list->size;
}

/*
 * Remove an element of the array list at a specific index.
 *
 * @param AL pointer to the array list
 * @param unsingend long index
 *
 * @return void pointer to the removed data or NULL if it doesn't exist
 */
void* al_del(AL *list, unsigned long index)
{
	assert(list);

	if(index >= list->size)
		return NULL;

	void *old = list->array[index];

	decreaseOne(list, index);

	return old;
}

/*
 * Add an array of void pointers to the array list.
 *
 * @param AL pointer to the array list
 * @param void pointer to the array
 *
 * @return void
 */
void al_addAll(AL *list, unsigned int data_size, void **data)
{
	assert(list);
	assert(data);
	assert(list->array);

	increase(list, list->size, data_size, data);

	// printf("Add %d\n", *(int *)data);
	// al_print(list);
	// printf("size %ld\n", list->size);
	// printf("memory_size %ld\n", list->memory_size);
}

void al_delRange(AL *list, unsigned long start, unsigned long end)
{
	decrease(list, start, end);
}

/*
 * Reverse the array list.
 *
 * @param AL pointer to the array list
 *
 * @return void
 */
void al_reverse(AL *list)
{
	assert(list);

	unsigned long i = 0, j = list->size;
	void *tmp;

	while(i < j)
	{
		j--;
//		printf("i = %ld, j = %ld\n", i, j);
//		printf("left: %p, right: %p\n", list->array[i], list->array[j]);
		tmp = list->array[i];
		list->array[i] = list->array[j];
		list->array[j] = tmp;
//		printf("left: %p, right: %p\n", list->array[i], list->array[j]);
		i++;
	}
}

/*
 * Deallocate memory of the array list.
 *
 * @param AL pointer to the array list
 *
 * @return void
 */
void al_clear(AL *list)
{
	assert(list);

	while(list->size)
	{
		list->freeFn(list->array[list->size]);
		list->array[list->size] = NULL;
		list->size--;
	}
	free(list->array);
	list->array = NULL;
	// list->size = 0;
	list->memory_size = 0;
}

/*
 * Print the array list to the console.
 *
 * @param AL pointer to the array list
 *
 * @return void
 */
void al_print(AL *list)
{
	assert(list);
	assert(list->printFn);

	unsigned long i;
	for(i = 0; i < list->size; i++)
	{
		printf("index: %ld data: ", i);
		list->printFn(list->array[i]);
	}
	puts("---");
}