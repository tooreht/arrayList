/**
 *  doublyLinkedList
 *
 *  https://github.com/tooreht/doublyLinkedList
 *
 *  @file   interactive.c
 *  @author Marc Zimmermann (tooreht@gmail.com)
 *  @date   December, 2012
 *  @brief  Interactive testing of doublyLinkedList.
 *
 *  This programm reads command lines from stdin and executes them.
 *  It's purpose is to interactively test the implementation of the data structure.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "al.h"

AL *list;
int *d1, *d2;

/**
 * Prints the usage options.
 *
 * @param void
 * @return void
 */
void usage(void)
{
    puts("arrayList usage:");
    puts("");
    puts("size\t\tprint the size");
    puts("next\t\tprint the next node");
    puts("prev\t\tprint the previous node");
    puts("print (p)\tprint the list");
    puts("info (i)\tprint list info");
    puts("hasNext\t\tprint if list has a next node");
    puts("hasPrev\t\tprint if list has a previous node");
    puts("reverse\t\treverse the list");
    puts("sort\t\tsort the list in ascending order");
    puts("pop\t\tpop the last element");
    puts("clear\t\tclear the whole list");
    puts("");
    puts("get 1 \t\tget the node at the index");
    puts("push 1 \tpush an integer to the end of the list");
    puts("find 1\t\tsearch for an integer in the list");
    puts("del 1\t\tdelete an integer in the list");
    puts("sad 1\t\tsearch and delete an integer in the list");
    puts("perform 10\tdo some performance testing with a number of elements");
    puts("");
    puts("set 1 2 \tset the node at the index 1 to 2");
    puts("fill 10 20\tfill the list with integers from 10 to 20");
    puts("find 10 1|2|3\tsearch for an integer in the list with a specific search mode");
    puts("\t\t(modes: 1=head to tail, 2=tail to head, 3=head and tail)");
    puts("sad 10 1|2|3\tsearch and delete an integer in the list with a specific search");
    puts("\t\tmode (modes: 1=head to tail, 2=tail to head, 3=head and tail)");
}

/**
 * Read from a file and print line by line.
 *
 * @param const char *file: file name
 * @return void
 */
void readFile(const char *file)
{
    FILE *fp; /* declare the file pointer */
    fp = fopen(file, "rt"); /* open the file for reading */

    if(fp == NULL)
    {
        printf("Cannot open file '%s'\n", file);
        exit(1);
    }

    size_t bytes_read;
    size_t nbytes = 8;

    char *line = (char *) malloc(nbytes + 1);

    while( (bytes_read = getline(&line, &nbytes, fp)) != -1)
    {
        printf("%s", line);
    }

    fclose(fp);  /* close the file prior to exiting the routine */
}

/**
 * Shows programm warranty information.
 *
 * @param void
 * @return void
 */
void showWarranty(void)
{
    puts("15. Disclaimer of Warranty.");
    puts("");
    puts("THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY");
    puts("APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT");
    puts("HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY");
    puts("OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,");
    puts("THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR");
    puts("PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM");
    puts("IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF");
    puts("ALL NECESSARY SERVICING, REPAIR OR CORRECTION.");
}

/**
 * Shows programm conditions.
 *
 * @param void
 * @return void
 */
void showConditions(void)
{
    readFile("LICENSE.md");
}

/**
 * Executes gpl commands.
 *
 * @param int nargs: number of arguments
 * @param char *command: pointer to command string
 * @param char option: option
 * @return void
 */
void executeGpl(int nargs, char *command, char option)
{
    switch(nargs)
    {
        case 2:
            if(!strcmp(command, "show"))
            {
                if(option == 'w')
                    showWarranty();
                else if(option == 'c')
                    showConditions();
            }
            break;
    }
}




/* =========================== User defined code =========================== */

/**
 * The data pointer is stored as a void pointer in each node of the list.
 * Thus three callback functions are needed to handle the generic
 * (userdefined) data structure:
 *
 * - printing the data
 * - freeing the data from memory
 * - comparing the data structures
 *
 * The function names don't matter, but the return types and parameters.
 *
 * In this programm simple integers are used:
 */

/**
 * This callback function is feed with the data pointer to print the data.
 * To access the integer, the void pointer is casted to an int pointer
 * and then dereferenced.
 *
 * @param void* data
 * @return void
 */
void printFn(void *data)
{
    if(data)
        printf("%d\n", *(int *) data);
    else
        printf("%p\n", data);
}

/**
 * This callback function is feed with the data pointer to free the data.
 *
 * @param void* data
 * @return void
 */
void freeFn(void *data)
{
    free(data);
}

/**
 * This callback function is feed with two data pointers.
 * To access the integers, the void pointer is casted to an int pointer
 * and then dereferenced.
 *
 * @param void* first_arg
 * @param void* second_arg
 * @return int
 *  -1: if first is greater than second
 *   0: if first and second are equal
 *   1: if first is lower than second
 */
int compareFn(void *first_arg, void *second_arg)
{
    int first = *(int *) first_arg;
    int second = *(int *) second_arg;

    if(first < second)
        return -1;
    else if(first == second)
        return 0;
    else
        return 1;
}

void info(AL *list)
{
    printf("--- Info ---\n");
    printf("memory size: %ld\n", list->memory_size);
    printf("size: %ld\n", list->size);
    printf("---\n");
} 

/**
 * Performs some tests with a dynamic number of elements.
 *
 * @param int elements: number of elements
 * @return void
 */
void perform(int elements)
{
    // clock_t start = clock();

    // int *integer;
    // void *data;

    // puts("fill dll with 'conventional' for loop");
    // int i;
    // for(i = 0; i < elements; i++)
    // {
    //     integer = malloc(sizeof(int));
    //     *integer = i;
    //     data = integer;
    //     dll_pushTail(list, data);
    // }

    // puts("print dll with 'iterator' while loop head to tail");
    // dll_head(list);
    // while(dll_hasNext(list))
    // {
    //     printf("%d\n", *(int*)list->curr->data);
    //     dll_next(list);
    // }

    // // puts("sort");
    // // dll_sort(list);
    // puts("reverse");
    // dll_reverse(list);
    // // dll_print(list);

    // puts("print dll with 'iterator' for loop tail to head");
    // Node *n = NULL;
    // for(n = dll_tail(list); dll_hasPrev(list); n = dll_prev(list))
    // {
    //     printf("%d\n", *(int*)n->data);
    // }

    // // puts("delete dll with for loop 'conventional' (dll_searchAndDelete is slow because of searching data)");
    // // for(i = 0; i < elements; i++)
    // // {
    // //  data = &i;
    // //  dll_searchAndDelete(list, data, 0);
    // // }

    // // puts("delete dll with while loop 'iterator' head to tail");
    // // dll_head(list);
    // // while(dll_hasNext(list))
    // // {
    // //  dll_next(list);
    // //  dll_popHead(list);
    // // }

    // int contains = 1;
    // void *d = &contains;
    // Node *node = dll_search(list, d, 1);
    // printf("contains %d\n", dll_contains(list, node));

    // puts("delete dll with 'iterator' while loop tail to head");
    // dll_tail(list);
    // while(dll_hasPrev(list))
    // {
    //     dll_prev(list);
    //     dll_popTail(list);
    // }

    // puts("print list");
    // dll_print(list);

    // double elapsed = ( (double)clock() - start ) / CLOCKS_PER_SEC;
    // printf("Performance with %d elements finished in %f s\n", elements, elapsed);
}

/**
 * Fills the list with a specific range of integers.
 *
 * @param int beg: beginning of range
 * @param int end: end of range
 * @return void
 */
void fill(int beg, int end)
{
    int *integer;
    void *data;
    int i;

    clock_t start = clock();

    if(beg > end)
    {
        for(i = beg; i >= end; i--)
        {
            integer = malloc(sizeof(int));
            *integer = i;
            data = integer;
            al_push(list, data);
        }

    }
    else
    {
        for(i = beg; i <= end; i++)
        {
            integer = malloc(sizeof(int));
            *integer = i;
            data = integer;
            al_push(list, data);
        }
    }

    double elapsed = ( (double)clock() - start ) / CLOCKS_PER_SEC;
    printf("Filling finished in %f s\n", elapsed);
}

/**
 * Executes dll commands.
 *
 * @param int nargs: number of arguments
 * @param char *command: pointer to command string
 * @param int arg1: argument one
 * @param int arg2: argument two
 * @return void
 */
void executeAl(int nargs, char *command, int arg1, int arg2)
{
    d1 = malloc(sizeof(int));
    d2 = malloc(sizeof(int));

    *d1 = arg1;
    *d2 = arg2;

    void *a1, *a2;

    a1 = d1;
    a2 = d2;

    if(!list)
    {
        list = al_create(10);
        list->compareFn = compareFn;
        list->freeFn = freeFn;
        list->printFn = printFn;
        // dll_registerCompareFn(list, compareFn);
        // dll_registerFreeFn(list, freeFn);
        // dll_registerPrintFn(list, printFn);
    }

    switch(nargs)
    {
        case 1:
            // if(!strcmp(command, "head"))
            // {
            //     Node *head = dll_head(list);
            //     printf("head\t%p %d\n", head, head ? *(int*)head->data : -1);
            // }
            // else if(!strcmp(command, "tail"))
            // {
            //     Node *tail = dll_tail(list);
            //     printf("tail\t%p %d\n", tail, tail ? *(int*)tail->data : -1);
            // }
            // else if(!strcmp(command, "curr"))
            // {
            //     Node *curr = dll_curr(list);
            //     printf("curr\t%p %d\n", curr, curr ? *(int*)curr->data : -1);
            // }
            // else if(!strcmp(command, "size"))
            // {
            //     printf("size\t%ld\n", dll_size(list));
            // }
            // else if(!strcmp(command, "hasNext"))
            // {
            //     if(dll_hasNext(list))
            //         puts("yes");
            //     else
            //         puts("no");
            // }
            // else if(!strcmp(command, "next"))
            // {
            //     Node *next = dll_next(list);
            //     printf("next\t%p %d\n", next, next ? *(int*)next->data : -1);
            // }
            // else if(!strcmp(command, "hasPrev"))
            // {
            //     if(dll_hasPrev(list))
            //         puts("yes");
            //     else
            //         puts("no");
            // }
            // else if(!strcmp(command, "prev"))
            // {
            //     Node *prev = dll_prev(list);
            //     printf("prev\t%p %d\n", prev, prev ? *(int*)prev->data : -1);
            // }
            if(!strcmp(command, "print") || !strcmp(command, "p"))
            {
                al_print(list);
            }
            else if(!strcmp(command, "info") || !strcmp(command, "i"))
            {
                info(list);
            }
            else if(!strcmp(command, "pop"))
            {
                void* data;
                if( (data = al_pop(list)) )
                    printf("%d\n", *(int *) data);
                else
                    printf("%p\n", data);
            }
            else if(!strcmp(command, "reverse"))
            {
                al_reverse(list);
            }
            // else if(!strcmp(command, "sort"))
            // {
            //     dll_sort(list);
            // }
            // else if(!strcmp(command, "popHead") || !strcmp(command, "poh"))
            // {
            //     dll_popHead(list);
            // }
            // else if(!strcmp(command, "popTail") || !strcmp(command, "pot"))
            // {
            //     dll_popTail(list);
            // }
            else if(!strcmp(command, "clear"))
            {
                clock_t start = clock();

                al_clear(list);

                double elapsed = ( (double)clock() - start ) / CLOCKS_PER_SEC;
                printf("Clearing finished in %f s\n", elapsed);

                list = NULL;
            }
            else if(!strcmp(command, "show"))
            {
                // do absolutely nothing
            }
            else
            {
                usage();
            }
            break;
        case 2:
            if(!strcmp(command, "get"))
            {
                void *data = al_get(list, *d1);
                if(data)
                    printFn(data);
                else
                    printf("Not found %p\n", data);
            }
            else if(!strcmp(command, "push"))
            {
                al_push(list, a1);
            }
            // else if(!strcmp(command, "pushHead") || !strcmp(command, "puh"))
            // {
            //     dll_pushHead(list, a1);
            // }
            // else if(!strcmp(command, "pushTail") || !strcmp(command, "put"))
            // {
            //     dll_pushTail(list, a1);
            // }
            else if(!strcmp(command, "del"))
            {
                al_del(list, *d1);
            }
            // else if(!strcmp(command, "find"))
            // {
            //     clock_t start = clock();
            //     Node *found = dll_search(list, a1, 1);
            //     double elapsed = ( (double)clock() - start ) / CLOCKS_PER_SEC;
            //     printf("searching finished after %f s\n", elapsed);
            //     if(found)
            //         printFn(found->data);
            //     else
            //         printf("Node with data %d couldn't be found\n", arg1);
            // }
            // else if(!strcmp(command, "sad"))
            // {
            //     dll_searchAndDelete(list, a1, 0);
            // }
            // else if(!strcmp(command, "perform"))
            // {
            //     perform(arg1);
            // }
            else
            {
                usage();
            }
            break;
        case 3:
            if(!strcmp(command, "set"))
            {
                al_set(list, *d1, a2);
            }
            else if(!strcmp(command, "add"))
            {
                al_add(list, *d1, a2);
            }
            else if(!strcmp(command, "delr"))
            {
                al_delRange(list, *d1, *d2);
            }
            // else if(!strcmp(command, "before"))
            // {
            //     Node *n = dll_search(list, a1, 1);
            //     if(n)
            //         list->curr = dll_addBefore(list, n, a2);
            //     else
            //         printf("Node with data %d couldn't be found\n", arg1);
            // }
            // else if(!strcmp(command, "after"))
            // {
            //     Node *n = dll_search(list, a1, 1);
            //     if(n)
            //         list->curr = dll_addAfter(list, n, a2);
            //     else
            //         printf("Node with data %d couldn't be found\n", arg1);
            // }
            // else if(!strcmp(command, "find"))
            // {
            //     clock_t start = clock();
            //     Node *found = dll_search(list, a1, arg2);
            //     double elapsed = ( (double)clock() - start ) / CLOCKS_PER_SEC;
            //     printf("Performance finished in %f s\n", elapsed);
            //     if(found)
            //         printFn(found->data);
            //     else
            //         printf("Node with data %d couldn't be found\n", arg1);
            // }
            // else if(!strcmp(command, "sad"))
            // {
            //     dll_searchAndDelete(list, a1, *d2);
            // }
            else if(!strcmp(command, "fill"))
            {
                fill(arg1, arg2);
            }
            else
            {
                usage();
            }
            break;
        default: usage();
    }
}

/**
 * Extracts the commands out of a line and executes them.
 *
 * @param char *line: char pointer to the line
 * @param size_t *nbytes: numbers of allocated charbytes
 * @return void
 */
void parseLine(char *line, size_t *nbytes)
{
    int nargs;
    char command[*nbytes], c;

    int a1 = 0, a2 = 0;

    nargs = sscanf(line, "%s %d %d", command, &a1, &a2);

    executeAl(nargs, command, a1, a2);

    nargs = sscanf(line, "%s %c", command, &c);

    executeGpl(nargs, command, c);
}

/**
 * Main:
 * Reads each line from stdin which is not an EOF.
 * Frees the allocated memory.
 *
 * @param int argc: number of arguments
 * @param char const *argv[]: pointer to arguments
 * @return int: success
 */
int main(int argc, char const *argv[])
{
    size_t bytes_read;
    size_t nbytes = 8;

    char *line = (char *) malloc(nbytes + 1);

    puts("---------- arrayList  Copyright (C) 2013  Marc Zimmermann ----------");
    puts("This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.");
    puts("This is free software, and you are welcome to redistribute it");
    puts("under certain conditions; type `show c' for details.");
    puts("");
    puts("Enter a command:");

    while( (bytes_read = getline(&line, &nbytes, stdin)) != -1)
    {
        parseLine(line, &bytes_read);
    }

    free(d1);
    free(d2);

    free(line);

    al_clear(list);

    return 0;
}
