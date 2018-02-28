//Armen Mouradyan, ID: 16861296
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LEN 300
#define HEAP_SIZE 127 //@Problem: 127 or 128?
#define HEADER_SIZE 2

char* first_fit(char* heap, int target_size);
void set_block_num(int n, char * p);
int get_block_num(char *p);
int get_block_size(char * p);
void set_block_size(int n, char * p);
int check_allocated(char * p);
void set_allocated(char * p);
void reset_allocated(char * p);
void split(int next_size, char* p, char* heap);

int main() {
    char heap[HEAP_SIZE]; //@Problem: Use char or ints
    char input[INPUT_LEN] = ""; //Input from user
    char* instruct = NULL;
    char* param1 = NULL;
    char* param2 = NULL;
    char* param3 = NULL;
    int block_count = 0; //@Problem: reuse deallocated blocks?

    //counter variables
    int i = 0;

    //initialize heap with header //@TODO:
   // heap[0] = 0; //block number //@Problem: start at 0 or 1?
    //heap[1] = 127 << 1; //@Problem: header doesn't count itself in size? What about footer? Padding?

    set_block_num(0, &heap[0]);
    set_block_size(127, &heap[1]);
    reset_allocated(&heap[1]);


    do { //process loop
        printf(">"); //print prompt for user input
        gets(input);//read input

        //split input into instruct and param
        instruct = strtok(input, " ");
        if (instruct != NULL) param1 = strtok(NULL, " ");
        if (param1 != NULL) param2 = strtok(NULL, " ");
        if (param2 != NULL) param3 = strtok(NULL, " ");

        if (strcmp(instruct, "allocate") == 0) { //@Problem: What happens when can't allocate? Told to give err msg
            int block_size = atoi(param1); //size of block wanted to allocated in bytes
            char *block;
            block_size += HEADER_SIZE; //inc block size by header size
            block = first_fit(heap, block_size); //find first fitting block
            if (block >= &heap[HEAP_SIZE] || block_size > HEAP_SIZE) { //if returned addr is larger than the last possible place //@Problem: Good enough check?
                printf("\nAllocation Failed\n");
            }
            else {
                int next_size = get_block_size(block + 1) - block_size; //next size = curr size - target size
                set_block_num(++block_count, block); //assign incremented block count to the block number of the block
                if (next_size > 2) set_block_size(block_size, block + 1); //set the block's block size
                else set_block_size(block_size + next_size, block + 1);
                set_allocated(block + 1); //set the allocated bit
                if (next_size > 2) split(next_size, block, heap); //split off a new unallocated block //@Problem: how to know not to split reallocated blocks?
                printf("\n%d\n", block_count); //@Problem: Can I just print the count instead of the actual value?
                // @Problem: what if allocation is not succesful? when wouldn't it be?
            }
        }

        else if (strcmp(instruct, "free") == 0) { //@Problem: How to handle invalid block numbers? How to make block nums invalid? Make index of bin array the num?
            int target_block_num = atoi(param1); //block number of block wanted to free
            char *p = &heap[1];
            while (p < &heap[HEAP_SIZE]) { //find the block
                if (*(p - 1) == target_block_num && check_allocated(p)) break;
                else p += get_block_size(p);
            }
            if (p >= &heap[HEAP_SIZE]) { //if block doesn't exist
                printf("\nInvalid block\n");
            }
            else {
                reset_allocated(p);
                set_block_num(0, p - 1);
            }
        }

        else if (strcmp(instruct, "blocklist") == 0) { //@Problem: Where is valid bit? How to handle non-word aligned?
            //takes no arguments
            printf("\n%-10s %-10s %-10s %-10s\n", "Size", "Allocated", "Start", "End");
            char *p = &heap[0];
            while (p < &heap[HEAP_SIZE]) {
                int start = p - &heap[0];
                printf("%-10d ", get_block_size(++p));
                if(check_allocated(p)) printf("%-10s ", "yes");
                else printf("%-10s ", "no");
                p = (p + get_block_size(p) - 1); //move to next block
                int end = p - &heap[0] - 1;
                //@Problem: What if end is beyond the array?
                printf("%-10d %-10d\n", start, end);
            }

        }

        else if (strcmp(instruct, "writeheap") == 0) {
            int target_block_num = atoi(param1); //block number of block wanted to write to
            char value = param2[0]; //@Problem: Can we assume we can just take the first char? Issue with null?
            int value_num = atoi(param3);
            char *p = &heap[1];

            while (p < &heap[HEAP_SIZE]) { //find the block
                if (*(p - 1) == target_block_num && check_allocated(p)) break;
                else p += get_block_size(p);
            }

            if (p >= &heap[HEAP_SIZE]) { //if block doesn't exist
                printf("\nInvalid block\n");
            }
            else if (get_block_size(p) - 2 < value_num) { //if too big to write to
                printf("\nWrite too big\n");
            }
            else { //write
                for (i = 1; i <= value_num; ++i) {
                    *(p + i) = value;
                }
            }
        }

        else if (strcmp(instruct, "printheap") == 0) {
            int target_block_num = atoi(param1); //number of block to be printed
            int print_len = atoi(param2); //number of bytes to print after start of block
            //if print_len is bigger than block len, only print the bytes in the block
            //don't print header, only payload
            char *p = &heap[1];

            while (p < &heap[HEAP_SIZE]) { //find the block
                if (*(p - 1) == target_block_num && check_allocated(p)) break;
                else p += get_block_size(p);
            }

            if (p >= &heap[HEAP_SIZE]) { //if block doesn't exist
                printf("\nInvalid block\n");
            }
            else {
                //if print_len is bigger than block len, only print the bytes in the block
                if (print_len > get_block_size(p) - 2) {
                    print_len = get_block_size(p) - 2;
                }

                printf("\n");
                for (i = 1; i <= print_len; ++i) { //print
                    printf("%c", *(p + i));
                }
                printf("\n");
            }

        }

        else if (strcmp(instruct, "printheader") == 0) {
            int target_block_num = atoi(param1); //number of block whose header is to be printed
            //Don't print if block is not allocated
            char *p = &heap[1];
            while (p < &heap[HEAP_SIZE]) { //find the block
                if (*(p - 1) == target_block_num && check_allocated(p)) break;
                else p += get_block_size(p);
            }
            if (p >= &heap[HEAP_SIZE]) { //if block doesn't exist
                printf("\nInvalid block\n");
            }
            else printf("\n%02X%02X\n", *(p - 1), (get_block_size(p) << 1) + 1);

        }
    } while (strcmp(input, "quit") != 0);

    return 0;
}

char* first_fit(char* heap, int target_size) {
    char size = (char)target_size;
    char *p = &heap[1]; //p points to first block size
    while ((p < &heap[HEAP_SIZE]) && //not passed end
            ((*p & 1) || //already allocated
            (get_block_size(p) < size))) { //size wanted to allocate is too large
        p += get_block_size(p); //iterate through implicit list
    }
    return p - 1; //return ptr to block num of found block
}

void set_block_num(int n, char * p){
    *p = (char) n;
}

int get_block_num(char *p){
    return (int) *p;
}

int get_block_size(char * p){
    return (int) ((unsigned char)*p >> 1);
}

void set_block_size(int n, char * p){
    *p = (char) n << 1 | (*p & 0x1);
}

int check_allocated(char * p){
    return (int) (*p & 0x1);
}

void set_allocated(char * p){
    *p = (*p | 0x1);
}
void reset_allocated(char * p){
    *p = (*p & 0xFE);
}

void split(int next_size, char* p, char* heap) { //@Problem: What to do when out of things to split
    p++; //have p point to the block size instead of the block num
    p += get_block_size(p); //move to next block
    if (p <= &heap[HEAP_SIZE]) {
        set_block_size(next_size, p); //set the size of the next block
        *(p - 1) = 0; //set block number of next block to zero since it's unallocated
        reset_allocated(p); //clear the allocated bit for the new block
    }
}