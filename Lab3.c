//Armen Mouradyan, ID: 16861296
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LEN 300 //@Problem: What size?
#define VM_ADDR_SIZE 16
#define MM_ADDR_SIZE 8
#define PTABLE_SIZE 8

//Converts address index to page number, returns page number, stores offset in offset_return
int addr_to_pn(int addr, int* offset_return);
//Converts page number to the first address in the page
int pn_to_addr_beg(int pn);
int find_lru();
void cache_page(int vmem_pn, int mmem_pn);
void update_lru(int mmem_addr);

struct Page{
    int vpn;
    int valid; //@Problem: is valid a keyword? Was highlighted purple for a second
    int dirty;
    int pn;
};

//@Problem: How to properly write globals?
static int vmem[VM_ADDR_SIZE]; //virtual memory
static int mmem[MM_ADDR_SIZE][2]; //main memory, first col is content, second col is least recently used count
static struct Page ptable[PTABLE_SIZE]; //page table

int main() {
    char input[INPUT_LEN] = ""; //Input from user
    char* instruct = NULL;
    char* param1 = NULL;
    char* param2 = NULL;

    //counter variables
    int i = 0;

    //initialize data structures
    //initialize vmem
    for (i = 0; i < VM_ADDR_SIZE; ++i) {
        vmem[i] = -1;
    }

    //initialize mmem
    for (i = 0; i < MM_ADDR_SIZE; ++i) {
        mmem[i][0] = -1; //initialize content
        mmem[i][1] = 0; //initialize LRU (Least Recently Used)
    }

    //initialize ptable
    for (i = 0; i < PTABLE_SIZE; ++i) {
        ptable[i].vpn = i; //@TODO: Change i to -1 after testing (or don't???)
        ptable[i].valid = 0; //@TODO: Change 1 to 0 after testing
        ptable[i].dirty = 0;
        ptable[i].pn = -1; //@TODO: Change i to -1 after testing
    }

    do { //process loop
        printf("$ "); //print prompt for user input
        gets(input);//read input

        //split input into instruct and param
        instruct = strtok(input, " ");
        if (instruct != NULL) param1 = strtok(NULL, " ");
        if (param1 != NULL) param2 = strtok(NULL, " ");

        //test
        //printf("\ninstruct: %s", instruct);
        //printf("\naddr: %s", param1);
        //printf("\nn: %s", param2);

        if (strcmp(instruct, "read") == 0) { //read from virtual address of param1 //@Problem: Assuming VM maps to PT
            //int target_addr = atoi(param1);
            int offset = 0;
            int target_pn = addr_to_pn(atoi(param1), &offset);//@TODO: atoi to strol

            //check to see if page is in mmem by checking valid in ptable
            //if valid
            if (ptable[target_pn].valid == 1) { //@Problem: Format for output?
                printf("\n%d", mmem[ptable[target_pn].pn + offset][0]);
                update_lru(ptable[target_pn].pn); //update LRU values

            }

            else { //if invalid
                if (!ptable[target_pn].dirty) { //if clean (dirty is clear)
                    //Find LRU page
                    int lru = find_lru();

                    //Replace the LRU page with the target page
                    cache_page(target_pn,lru);
                }
                else { //if dirty
                    //@TODO
                }
            }
        }
        else if (strcmp(instruct, "write") == 0) {
            //@TODO
        }
        else if (strcmp(instruct, "showmain") == 0) { //output page of param1 in mmem
            int target_addr = pn_to_addr_beg(atoi(param1)); //@TODO: atoi to strol
            printf("\n%-10s %-10s", "Address", "Contents");
            printf("\n%-10d %-10d", target_addr, mmem[target_addr][0]);
            printf("\n%-10d %-10d", target_addr + 1, mmem[target_addr + 1][0]);
        }
        else if (strcmp(instruct, "showdisk") == 0) {//out page of param1 in vmem //@Problem: Just printing v and m addresses? Format?
            int vmem_addr = pn_to_addr_beg(atoi(param1)); //@TODO: atoi to strol
            int mmem_pn = -1;
            int mmem_addr = -1;

            for (i = 0; i < PTABLE_SIZE; ++i) {
                if (ptable[i].vpn == vmem_addr) {
                    if (ptable[i].valid) {
                        mmem_pn = ptable[i].pn;
                    }
                }
            }

            mmem_addr = pn_to_addr_beg(mmem_pn);

            printf("\n%-10s %-10s", "Address", "Contents"); //@Problem: Call the cols these names?
            printf("\n%-10d %-10d", vmem_addr, mmem_addr);
            printf("\n%-10d %-10d", vmem_addr + 1, mmem_addr + 1);
        }
        else if (strcmp(instruct, "showptable") == 0) { //output ptable
            printf("\n%-10s %-10s %-10s %-10s", "VPageNum", "Valid", "Dirty", "PN");
            for (i = 0; i < PTABLE_SIZE; ++i) {
                printf("\n%-10d %-10d %-10d %-10d", ptable[i].vpn, ptable[i].valid, ptable[i].dirty, ptable[i].pn);
            }
        }

        printf("\n");
    } while (strcmp(input, "quit") != 0);

    return 0;
}

int addr_to_pn(int addr, int* offset_return) {
    if (offset_return != NULL) *offset_return = addr%2; //store the remainder
    return addr/2; //integer division
}

int pn_to_addr_beg(int pn) {
    return pn*2;
}

int find_lru() {
    int lru = 0; //least recently used page, initialized to first page number
    int lru_count = mmem[0][1]; //value of lru column, initialized to the first page's count

    //counter variables
    int i = 0;

    for (i = 0; i < MM_ADDR_SIZE; i = i + 2) {
        if (mmem[i][1] > lru_count) { //if found a lesser used page, larger values are less used
            lru = addr_to_pn(i, NULL);
            lru_count = mmem[i][1];
        }
    }

    return lru;
}

void cache_page(int vmem_pn, int mmem_pn) {
    int vmem_addr = pn_to_addr_beg(vmem_pn);
    int mmem_addr = pn_to_addr_beg(mmem_pn);

    //counter variables
    int i = 0;

    //move from vmem to mmem
    mmem[mmem_addr][0] = vmem[vmem_addr];
    mmem[mmem_addr + 1][0] = vmem[vmem_addr + 1];

    //Update LRU values
    update_lru(mmem_addr);

    //Set pn in ptable for vmem page
    ptable[vmem_pn].pn = mmem_pn;

    //Set valid in ptable for vmem page
    ptable[vmem_pn].valid = 1;

    //Clear old mmem page's valid
    ptable[mmem_pn].valid = 0;
}

void update_lru(int mmem_addr) {
    //counter variables
    int i = 0;

    for (i = 0; i < MM_ADDR_SIZE; ++i) { //incrememnt all lru values
        mmem[i][1] += 1;
    }

    //make the page passed in the most recently used
    mmem[mmem_addr][1] = 0;
    mmem[mmem_addr + 1][1] = 0;
}