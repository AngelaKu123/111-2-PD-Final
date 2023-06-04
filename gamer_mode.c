#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "database.h"
#include "gamer_mode.h"

int* qid_rand_choose(void);
void gamer_mode(void);

void gamer_mode(void){
    //intization
    char string[100];

    //cp_output();

    //if(search_ID_ques(4) == NULL){printf("problem???????\n");}

    int *id_choose = qid_rand_choose();
    if (id_choose == NULL) {
        //End this game!
    }

    printf("\nHello! Welcome to user mode! Hope you guys have some fun!\n");
    int ccc = 0, user_S_ans = 0, score = 0, keep_answer_right = 0;
    for (int *ptr = id_choose; ptr != NULL && ccc < 10; ptr++) {
        struct ques *temp_ques = search_ID_ques(*ptr);
        printf("\nQuestion id: %d\n", temp_ques->qid);
        printf("Question: %s\n",temp_ques->q_content);
        printf("Answer: %d\n", temp_ques->ans);
        printf("How many people answered the question: %d\n", temp_ques->answered_num);
        printf("Correct percent: %.2f\n", temp_ques->correct_percent);
        printf("How many people got it right: %d\n", temp_ques->correct_num);

        printf("Now type your answer: ");
        scanf("%d", &user_S_ans);
        if (user_S_ans != temp_ques->ans) {
            printf("What a pity! You got it wrong!\n");
            keep_answer_right = 0;
            temp_ques->correct_percent = ((temp_ques->correct_num * 1.0 ) / ((temp_ques->answered_num) + 1));//remember to multiply 100
        } else {
            printf("Amazing! You got the question right!\n");
            keep_answer_right++;
            if (keep_answer_right >= 2)
                printf("Wow! You answered %d questions in a row!\n", keep_answer_right);
            score += 10;
            temp_ques->correct_num++;

            temp_ques->correct_percent = ((temp_ques->correct_num * 1.0) / ((temp_ques->answered_num) + 1));
            printf("debug: %.2f\n", temp_ques->correct_percent);
        }
        temp_ques->answered_num++;
        ccc++;
    }
    printf("\nCongratulations! You got %d points!\n", score);

    //gcc .\jsf_test_gamer_mode.c .\database.c -o jsf_test_gamer_mode
    //C:\Users\g5368\Desktop\Csie_exercises_projects\ProgramDesignSem2\final_project_test\111-2-PD-Final-main
}

int* qid_rand_choose(void) {    //return the pointer point to array which record the q_id been choosen
    srand( time(NULL) );
    int* record = malloc(10 * sizeof(int));
    int i , j;
    int max = max_id();
    if (num_question() <10) {
        printf("The number of questions is not enough!(< 10)\n");
        return NULL;
    }
    for (i = 1; i <= 10; i++) {
        do {
            do {
                record[i - 1] = rand() % (max - 1 + 1) + 1;
            } while ( search_ID_ques(record[i - 1]) == NULL);
             
            for (j = 1; j < i; j++) {
                if(record[i - 1] == record[j - 1]){
                    break;
                }
            }
        } while (j != i);
        //printf("%d ", record[i - 1]);
    }
    //printf("\n");
    return record;
}
