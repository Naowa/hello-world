#include <stdio.h>
#include <string.h>
//#include <rpcndr.h>
#include <stdlib.h>

#define MAX_REQ_COURSES 25
#define MAX_COURSES 25
#define MAX_PREREQS 5
#define MAX_COURSE_NAME_LEN 21
#define MAX_PREREQ_LINE_LEN 126
#define SET_STR_FLAG "0"
#define CLEAR_STR_FLAG "1"
#define MAX_YEARS 10
#define MAX_QUARTERS 30
#define QUARTERS 3

void newline_to_terminal(char* str);

int main(int argc, char *argv[]) {
    char *offering_file = argv[1];
    char *prereq_file = argv[2];
    char *req_file = argv[3];
    char req_courses[MAX_REQ_COURSES][MAX_COURSE_NAME_LEN + 2];
    FILE *curr_file;
    char buff[255];
    int req_courses_len = 0; // req_courses_len is the size of req_courses
    char prereq_line[MAX_PREREQ_LINE_LEN + 1];
    char courses_to_take[MAX_COURSES][MAX_PREREQS + 3][MAX_COURSE_NAME_LEN + 2];
    int courses_to_take_courselen = 0;
    char prereq_table[MAX_COURSES][MAX_PREREQS + 2][MAX_COURSE_NAME_LEN + 2];
    int prereq_table_courselen = 0;
    int prereq_count = 0;
    char taken[MAX_COURSES][MAX_COURSE_NAME_LEN + 2];
    int taken_len = 0;
    char fall_offerings[MAX_COURSES][MAX_COURSE_NAME_LEN + 2];
    char winter_offerings[MAX_COURSES][MAX_COURSE_NAME_LEN + 2];
    char spring_offerings[MAX_COURSES][MAX_COURSE_NAME_LEN + 2];
    int fall_count = 0;
    int winter_count = 0;
    int spring_count = 0;
    char offering_line[MAX_COURSE_NAME_LEN + 2];

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
        //courses_to_take[i][0] = CLEAR_STR_FLAG;
        //courses_to_take[i][2] = "0";
        strcpy(courses_to_take[i][0], CLEAR_STR_FLAG);
        strcpy(courses_to_take[i][2], "0");
    }

    //add required courses to courses_to_take
    for (int i = 0; i < req_courses_len; ++i) {
        //courses_to_take[i][0] = SET_STR_FLAG;
        //courses_to_take[i][1] = req_courses[i];
        strcpy(courses_to_take[i][0], SET_STR_FLAG);
        strcpy(courses_to_take[i][1], req_courses[i]);
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
        // Store into prereqs table
        int i = 0;

        if (word != NULL) {
            //prereq_table[prereq_table_courselen][i++] = word; //if I know it has a \0 then strcpy, otherwise strncpy
            //prereq_table[prereq_table_courselen][i++] = "0"; //number of prereqs
            strcpy(prereq_table[prereq_table_courselen][i++], word);
            strcpy(prereq_table[prereq_table_courselen][i++], "0");
            ++prereq_count;
            word = strtok(NULL, " ");
        }
        while (word != NULL) {
           // printf("%s ", word);
            newline_to_terminal(word);
            //prereq_table[prereq_table_courselen][i++] = word;
            strcpy(prereq_table[prereq_table_courselen][i++], word);
            int x = atoi(prereq_table[prereq_table_courselen][1]);
            itoa( x + 1, prereq_table[prereq_table_courselen][1], 10);
            word = strtok(NULL, " ");
            newline_to_terminal(word);
        }
        ++prereq_table_courselen;
    }
    /*
    //test print
    for (int i = 0; i < MAX_COURSES; ++i) {
        for (int j = 0; j < MAX_PREREQS + 2; ++j) {
            printf("%s ", prereq_table[i][j]);
        }
        printf("\n");
    }

    printf("%d", prereq_count);
     */

    //close prereq
    fclose(curr_file);

    //open offerings
    curr_file = fopen(offering_file, "r");

    //@TODO: Read and store offerings
    /*
    while (fgets(offering_line, MAX_COURSE_NAME_LEN + 2, curr_file) != NULL) {
        int len = strlen(offering_line);
        char* title = strtok(offering_line, " ");
        int quart = atoi(offering_line);
        printf("%s %d\n", title, quart);
    }
     */
    while (fscanf(curr_file, "%s", offering_line) == 1) {
        char quart[2];
        int quart_i = 0;
        fscanf(curr_file, "%s", quart);
        quart_i = atoi(quart);
        //printf("%s %d\n", offering_line, quart_i);
        switch (quart_i) {
            case 1:
                //fall_offerings[fall_count++] = offering_line;
                strcpy(fall_offerings[fall_count++], offering_line);
                break;
            case 2:
                strcpy(winter_offerings[winter_count++], offering_line);
                break;
            case 3:
                strcpy(spring_offerings[spring_count++], offering_line);
                break;
            default: break;
        }
    }
    /*
    //test fall offerings
    printf("\n");
    for (int i = 0; i < fall_count; ++i) {
        printf("%s ", fall_offerings[i]);
    }

    //test fall offerings
    printf("\n");
    for (int i = 0; i < winter_count; ++i) {
        printf("%s ", winter_offerings[i]);
    }

    //test fall offerings
    printf("\n");
    for (int i = 0; i < spring_count; ++i) {
        printf("%s ", spring_offerings[i]);
    }
    */

    //close offerings
    fclose(curr_file);

    //@TODO: Generate courses_to_take
    for (int i = 0; i < courses_to_take_courselen; ++i) {
        char* target = courses_to_take[i][1];
        int target_prereq_num = atoi(courses_to_take[i][2]);

        for (int j = 0; j < prereq_table_courselen; ++j) {
            if (strcmp(target, prereq_table[j][0]) == 0) {

                int prereq_num = atoi(prereq_table[j][1]);

                for (int x = 2; x < prereq_num + 2; ++x) {
                    int exists = 0;
                    char *curr_course = prereq_table[j][x];

                    for (int k = 0; k < courses_to_take_courselen; ++k) {
                        if (strcmp(curr_course, courses_to_take[k][1]) == 0) {
                            exists = 1;
                        }
                    }

                    if (!exists) {
                        //add prereq to the courses_to_take
                        strcpy(courses_to_take[courses_to_take_courselen][0], "0");
                        strcpy(courses_to_take[courses_to_take_courselen][1], curr_course);
                        strcpy(courses_to_take[courses_to_take_courselen++][2], "0");
                    }

                    //add prereq to target's prereqs and inc count
                    strcpy(courses_to_take[i][target_prereq_num + 3], curr_course);
                    ++target_prereq_num;
                }
            }
        }

        itoa(target_prereq_num,courses_to_take[i][2], 10); //reassign prereq count to courses_to_take
    }

    //test courses_to_take
    for (int i = 0; i < courses_to_take_courselen; ++i) {
        int prereq_num = atoi(courses_to_take[i][2]);
        printf("%s %s %d", courses_to_take[i][0], courses_to_take[i][1], prereq_num);
        for (int j = 3; j < prereq_num + 3; ++j) {
            printf(" %s", courses_to_take[i][j]);
        }
        printf("\n");
    }

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