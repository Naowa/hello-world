//Armen Mouradyan, ID: 16861296
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LEN 300
#define HEAP_SIZE 127
#define HEADER_SIZE 2

int main() {
    char heap[HEAP_SIZE]; //@Problem: Use char or ints
    char input[INPUT_LEN] = ""; //Input from user
    char* instruct = NULL;
    char* param1 = NULL;
    char* param2 = NULL;
    char* param3 = NULL;
    char curr_block_count = 0; //don't reuse deallocated blocks

    //counter variables
    int i = 0;

    //initialize heap with header //@TODO:
    heap[0] = 0; //block number //@Problem: start at 0 or 1? If 0, not allocated? How to mark one invalid?
    heap[1] = 127; //@Problem: header doesn't count itself in size? What about footer? Padding?


    do { //process loop
        printf(">"); //print prompt for user input
        gets(input);//read input

        //split input into instruct and param
        instruct = strtok(input, " ");
        if (instruct != NULL) param1 = strtok(NULL, " ");
        if (param1 != NULL) param2 = strtok(NULL, " ");
        if (param2 != NULL) param3 = strtok(NULL, " ");

        if (strcmp(instruct, "allocate") == 0) { //@Problem: What happens when can't allocate?
            int block_size = atoi(param1); //size of block wanted to allocated in bytes
            //@TODO: inc block size by header size
            //@TODO: allocate
            //@TODO: print block_num if allocated
            //@TODO: inc block_num if allocated
        }
        else if (strcmp(instruct, "free") == 0) { //@Problem: How to handle invalid block numbers? How to make block nums invalid? Make index of bin array the num?
            int target_block_num = atoi(param1); //block number of block wanted to free
            //@TODO: free
            //@TODO: invalidate block num used if free
        }
        else if (strcmp(instruct, "blocklist") == 0) { //@Problem: Where is valid bit? How to handle non-word aligned?
            //takes no arguments
            //@TODO: print block infomration

        }
        else if (strcmp(instruct, "writeheap") == 0) {
            int target_block_num = atoi(param1); //block number of block wanted to write to
            char value = param2[0]; //@Problem: Can we assume we can just take the first char? Issue with null?
            int value_num = atoi(param3);
            //@TODO: if larger than payload size, don't write to block, print out "Write too big"
            //@TODO: Otherwise, write to block
        }
        else if (strcmp(instruct, "printheap") == 0) {
            int target_block_num = atoi(param1); //number of block to be printed
            int print_len = atoi(param2); //number of bytes to print after start of block
            //if print_len is bigger than block len, only print the bytes in the block
            //don't print header, only payload
            //@TODO: print block

        }
        else if (strcmp(instruct, "printheader") == 0) {
            int target_block_num = atoi(param1); //number of block whose header is to be printed
            //@TODO: print in hex
            //@Problem: example for this instruct prints out 0515 instead of 05A or 05C
            //@Problem: What to do if block is not allocated
            //Don't print if block is not allocated

        }
    } while (strcmp(input, "quit") != 0);

    return 0;
}