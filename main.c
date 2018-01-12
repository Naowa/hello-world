#include <stdio.h>
#include <string.h>
//#include <rpcndr.h>
#include <stdlib.h>

#define MAX_REQ_COURSES 500
#define MAX_COURSES 500
#define MAX_PREREQS 5
#define MAX_COURSE_NAME_LEN 20
#define MAX_PREREQ_LINE_LEN 126
#define SET_STR_FLAG "0"
#define CLEAR_STR_FLAG "1"

void newline_to_terminal(char* str);

int main(int argc, char *argv[]) {
    char *offering_file = argv[1];
    char *prereq_file = argv[2];
    char *req_file = argv[3];
    char req_courses[MAX_REQ_COURSES][MAX_COURSE_NAME_LEN];
    FILE *curr_file;
    char buff[255];
    int req_courses_len = 0; // req_courses_len is the size of req_courses
    char prereq_line[MAX_PREREQ_LINE_LEN + 1];
    char* courses_to_take[MAX_COURSES][MAX_PREREQS+3];
    int courses_to_take_courselen = 0;
    char* prereq_table[MAX_COURSES][MAX_PREREQS + 2];
    int prereq_table_courselen = 0;
    char* taken[MAX_COURSES];
    int taken_len = 0;

    //open req
    curr_file = fopen(req_file,"r");
    //generate of array of required courses
    while (fscanf(curr_file, "%s", req_courses[req_courses_len++]) == 1); //while fscanf returns valid flag
    req_courses_len--; //because the last string put in the list will be garbage
    //close req
    fclose(curr_file);

    /*
    //@Test: Print reqs
    printf("Reqs:\n");
    for (int i = 0; i < req_courses_len; ++i) {
        printf("%s ", req_courses[i]);
    }
     */

    //clear all flags in courses_to_take (in index 0) and set the prereq count in index 2 to "0"
    //index 0 flag -> if courses at index 1 has been scheduled ("0" for no "1" for yes)
    //index 2 -> number of prereq courses in subsequent indices
    for (int i = 0; i < MAX_COURSES; ++i){
        courses_to_take[i][0] = CLEAR_STR_FLAG;
        courses_to_take[i][2] = "0";
    }

    //add required courses to courses_to_take
    for (int i = 0; i < req_courses_len; ++i) {
        courses_to_take[i][0] = SET_STR_FLAG;
        courses_to_take[i][1] = req_courses[i];
        ++courses_to_take_courselen;
    }

    //open prereq
    curr_file = fopen(prereq_file, "r");
    //read from prereq
    //printf("\n\nPrereqs:\n");
    while (fgets(prereq_line, MAX_PREREQ_LINE_LEN + 1, curr_file) != NULL) { //read in a line
        //char* target_course = "";
        char* word = strtok(prereq_line, " "); //read in a word per line
        newline_to_terminal(word);
        //strncpy (target_course, word, MAX_COURSE_NAME_LEN);
        //@TODO: Store into prereqs table
        int i = 0;

        if (word != NULL) {
            prereq_table[prereq_table_courselen][i++] = word;
            prereq_table[prereq_table_courselen][i++] = "0"; //number of prereqs
            word = strtok(NULL, " ");
        }
        while (word != NULL) {
           // printf("%s ", word);

            prereq_table[prereq_table_courselen][i++] = word;
            //@Problem: segmentation fault // do all compilers fill undefined chars with NULL?
            //itoa(atoi(prereq_table[prereq_table_courselen][1]) + 1, prereq_table[prereq_table_courselen][1], 10);
            word = strtok(NULL, " ");
            newline_to_terminal(word);
        }
        ++prereq_table_courselen;
    }
    //close prereq
    fclose(curr_file);

    //@TODO: Read and store offerings

    //@TODO: Generate courses_to_take

    //@TODO: Generate Schedule

    return 0;
}

void newline_to_terminal(char* str) {
    if (str != NULL) {
        int i = 0;
        while (str[i] != '\n' && str[i] != '\0') {
            ++i;
        }
        if (str[i] == '\n') {
            str[i] = '\0';
        }
    }
}