//Armen Mouradyan, ID: 16861296
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_ELEMS 10
#define MAX_INT_STR 10

int search (int arr[], int left, int right, int target, pid_t main_pid);
void s_search(int arr[], int left, int right, int target);
void p_search(int arr[], int left, int right, int target, int p[]);

int main(int argc, char *argv[]) {
    char *arr_file = argv[1];
    int target = atoi(argv[2]); //@Problem: Possible conversion error
    FILE *curr_file;
    int arr[MAX_ELEMS];
    int arr_len = 0;
    char temp[MAX_INT_STR];
    int fork_count = 0;
    int child_status = 0;
    pid_t primary_parent_pid = getpid();
    pid_t pid;
    int target_index = -1;

    //loop counters
    int i = 0;

    //open arr_file
    curr_file = fopen(arr_file, "r");

    //read in array
    while (arr_len < MAX_ELEMS) {
        fscanf(curr_file, "%s", temp);
        arr[arr_len++] = strtol(temp, NULL, 10); //@Problem: converting long int to int
    }

    //close arr_file
    fclose(curr_file);

    /*
    //Test arr
    for (i = 0; i < arr_len; ++i) {
        printf("%d ", arr[i]);
    }
     */

    //printf("\nMain PID: %d", getpid());

    //printf("\nTarget: %d", target);

    //printf("\nMain PID: %d, \tTarget: %d\n", getpid(), target);

    /*
    //Test search
    int value = search(arr, 0, arr_len - 1, target, &fork_count);
    //for (i = 0; i < fork_count; ++i) { //@Problem: would this work?
    //    wait(&child_status);
    //}
    (value == -1) ? printf("\nvalue not present") : printf("\nFound at index %d", value);
     */

    //Test Search 2
    //int index = search(arr, 0, arr_len - 1, target, getpid());
    //(index == -1) ? printf("\nvalue not present\n") : printf("\nFound at index %d\n", index);

    /*
    //Test Search 3 //assuming pids are sequential
    for (i = 0; i < arr_len; ++i) {
        pid = fork();
        if (pid == 0) {
            if (target == arr[pid%(arr_len - 1)]) {
                exit(pid%(arr_len - 1));
            }
            else {
                exit(-1);
            }
        }
    }
    for (i = 0; i < arr_len; ++i) {
        wait(&child_status);
        if(child_status >= 0) {
            target_index = child_status;
        }
    }
    (target_index == -1) ? printf("\nvalue not present") : printf("\nFound at index %d", target_index);
    */

    /*
    //Test Search 4
    s_search(arr, 0, arr_len - 1, target);
    wait(&child_status);
    if (child_status >= 0) {
        printf("\nIndex: %d\n", child_status);
    }
    else {
        printf("Not found");
    }
    //(target_index == -1) ? printf("\nvalue not present\n") : printf("\nFound at index %d\n", target_index);
     */


    //Test Search 5
    int p[2];
    int not_found = -1;
    pipe(p);
    //printf("p[0]: %d, p[1]: %d\n", p[0], p[1]);
    /*
    close(p[0]);
    write(p[1], &not_found, sizeof(int));
    close(p[1]);
    */

    pid = fork();
    if (pid == 0) p_search(arr, 0, arr_len - 1, target, p);
    wait(&child_status);
    //close(p[0]);
    close(p[1]);
    read(p[0], &target_index, sizeof(int));
    printf("Search Output: %d\n", target_index); //@problem: pid values keep going after this
    return 0;
}

void p_search(int arr[], int left, int right, int target, int p[]) {
    int mid = (left+right)/2;
    if (left < right) {
        int child_status1 = 0;
        int child_status2 = 0;
        pid_t pid1;
        pid1 = fork();
        if (pid1 == 0) { //child 1
            p_search(arr, left, mid, target, p);
        }
        else { //parent
            wait(&child_status1);
            p_search(arr, mid + 1, right, target, p);
        }
    }
    else if (left == right) {
        printf("pid %d, value: %d\n", getpid(), arr[mid]);
        fflush(stdin);
        if (arr[mid] == target) {
            close(p[0]);
            write(p[1], &mid, sizeof(int));
            close(p[1]);
        }
        //printf("p[0]: %d", p[0]);
        exit(0); //@Problem: need this to exit but 'causes an issue
    }
}

void s_search(int arr[], int left, int right, int target) {
    int mid = (left+right)/2;
    if (left < right) {
        int child_status1 = 0;
        int child_status2 = 0;
        pid_t pid2;
        pid_t pid1;
        pid1 = fork();
        if (pid1 != 0) pid2 = fork();
        if (pid1 == 0) { //child 1
            s_search(arr, left, mid, target);
        }
        else if (pid2 == 0) { //child 2
            s_search(arr, mid + 1, right, target);
        }
        else {
            wait(&child_status1);
            wait(&child_status2);
        }
    }
    else if (left == right) {
        if (arr[mid] == target) {
            printf("FOUND AT %d", mid);
            exit(mid);
        }
        else {
            exit(-1);
        }
    }
    else {
        exit(-1);
    }
}

int search (int arr[], int left, int right, int target, pid_t main_pid) { //@Problem: arr?
    printf("Entering function PID: %d\n", getpid());
    int mid = (left+right)/2;
    if (left < right) { //can split
        int child_status1 = 0;
        int child_status2 = 0;
        pid_t pid1;
        pid_t pid2;
        pid1 = fork();
        if (pid1 != 0) pid2 = fork();
        if (pid1 == 0) { //child 1
            int value1 = search(arr, left, mid, target, main_pid);
            printf("Exiting after search PID: %d\n", getpid());
            exit(value1);
        }
        else if (pid2 == 0) { //child 2
            int value2 = search(arr, mid + 1, right, target, main_pid);
            printf("Exiting after search PID: %d\n", getpid());
            exit(value2);
        }
        else { // (pid1 != 0 && pid2 != 0) { //parent //@Problem: condition is always true

            wait(&child_status1);
            wait(&child_status2);
            //if primary parent -> return
            if (getpid() == main_pid) {
                //printf("\nIn main process and returning result %d", getpid());
                printf("Returning from function as primary parent PID: %d\n", getpid());
                if (child_status1 >= 0) { //child 1 found it
                    printf("Returning value: %d\n", child_status1);
                    return child_status1;
                } else { //either child 2 found it or neither found it, in which case child_status2 should hold -1
                    printf("Returning value: %d\n", child_status2);
                    return child_status2;
                }
            }
            //if non-primary parent -> exit
            else {
                printf("Exiting as non-primary parent PID: %d\n", getpid());
                if (child_status1 >= 0) { //child 1 found it
                    exit(child_status1);
                } else { //either child 2 found it or neither found it, in which case child_status2 should hold -1
                    exit(child_status2);
                }
            }
        }
    }
    else { //cannot split
        printf("Returning from comparison PID: %d\n", getpid());
        if (arr[mid] == target) {
            return mid;
        }
        else {
            return -1;
        }
    }
    /*
    int child_status = 0;
    int mid = (left + right) / 2;
    if (left < right) {
        pid_t pid;
        pid = fork(); //@Problem: Fork it do each split
        ++(*fork_count);
        if (pid == 0) {
            int value1 = search(arr, left, mid, target, fork_count);
            if (value1 >= 0) {
                exit(value1); //try exiting here
            }
            else {
                exit(-1);
            }
        }
        else {
            int value2 = search(arr, mid + 1, right, target, fork_count);
            if (value2 >= 0) {
                return value2;
            }
        }
    }
//either wait here or split children, child1 running one half child 2 running other, wait twice then
    wait(&child_status);
    if (arr[mid] == target) {
        return mid;
    }
    else {
        return -1;
    }*/
}