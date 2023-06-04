#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "database.h"
#include "gamer_mode.h"
#include "manager_mode.h"


void main(){

    //database input from file
    FILE *fptr;
    fptr = fopen("test.txt", "r");
    if (fptr == NULL) {
        printf("fail to open!\n");
        return;
    }

    char temp_q_content[501];
    int temp_ans = 0, temp_answered_num = 0, temp_correct_num = 0;
    float temp_correct_percent = 0.0;
    while (fgets(temp_q_content, 500, fptr) != NULL) {
        temp_q_content[strlen(temp_q_content) - 1] = '\0';//clear \n char
        fscanf(fptr, "%d ", &temp_ans);
        fscanf(fptr, "%f ", &temp_correct_percent);
        fscanf(fptr, "%d ", &temp_answered_num);
        fscanf(fptr, "%d ", &temp_correct_num);

        insert_newques( temp_q_content, temp_ans, temp_correct_num, temp_answered_num, temp_correct_num);
    }

    int exit=0;
    while(!exit){
        int ins;
        printf("choose mode: 1:gamer_mode, 2:manager_mode, 3:exit:\ninput:");
        scanf("%d",&ins);
        switch (ins)
        {
        case 1:
            gamer_mode();
            break;
        case 2:
            manager_mode();
            break;
        case 3:
            exit=1;
            break;
        
        default:
            continue;
            break;
        }
    }

    fclose(fptr);
    //output 
    FILE *fptr_out;
    fptr_out = fopen("test.txt", "w");
    if (fptr_out == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }


    for (int i = 1; i <= max_id(); i++) { //there some bug that will fprintf id = 0
        struct ques *temp_ques = search_ID_ques(i);
        if(temp_ques == NULL){
            continue;
        }
        fputs(temp_ques->q_content, fptr_out);
        fprintf(fptr_out, "\n%d\n", temp_ques->ans);

        //renew answered_num and correct_num
        fprintf(fptr_out, "%.2f\n", temp_ques->correct_percent);
        fprintf(fptr_out, "%d\n", temp_ques->answered_num);
        fprintf(fptr_out, "%d\n", temp_ques->correct_num);
    }

    fclose(fptr_out);

    return;
}