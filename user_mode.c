#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>   //cooperate with rand()
//#include "database.h"

char* qid_rand_choose(void);
void Bubble_sort(int arr[], int num);

#ifndef DATABASE_H
    #define DATABASE_H

    #define len 500

    // Public functions
    struct ques* search_ID_ques(int id);
    int max_id();
    void keyword_output(char* keyword);

    struct ques {
        int qid;
        char q_content[len];
        int ans;
        float correct_percent;
        int answered_num;        // Number of times answered
        int correct_num;         // Number of times answered correctly
    };

    struct node {
        struct ques que;         // Question structure
        int height_qid;          // Height of the node in the qid tree
        struct node* linklist_next;  // Next node in the linked list
        struct node* linklist_prior; // Previous node in the linked list
        struct node* lchild_qid;     // Left child node in the qid tree
        struct node* rchild_qid;     // Right child node in the qid tree
    };

    struct ques_set {
        int num_ques;
        struct ques* question_set[]; // Array of question pointers
    };

#endif


int main(){

    FILE *fptr;
    fptr = fopen("test.txt", "r");
    if (fptr == NULL) {
        printf("fail to open!\n");
        return 0;
    }
    struct node* first = malloc(sizeof(struct node));
    first->linklist_prior = NULL;
    //scan info from files
    struct node* ptr = first;
    while (fscanf(fptr, "%d ", &ptr->que.qid) != EOF) {
        fgets(ptr->que.q_content, 500, fptr);
        ptr->que.q_content[strlen(ptr->que.q_content) - 1] = '\0';//clear \n char
        fscanf(fptr, "%d ", &ptr->que.ans);
        fscanf(fptr, "%f ", &ptr->que.correct_percent);
        fscanf(fptr, "%d ", &ptr->que.answered_num);
        fscanf(fptr, "%d ", &ptr->que.correct_num);

        struct node* temp = ptr;
        struct node* new_node = malloc(sizeof(struct node));
        ptr->linklist_next = new_node;
        ptr = ptr->linklist_next;
        ptr->linklist_prior = temp;
        ptr->linklist_next = NULL;
    }

    int *id_choose = qid_rand_choose();
    if (id_choose == NULL) {
        return 0;//End this game!
    }

    //if choose the deleted question?
    Bubble_sort(id_choose, 10);

    printf("Hello! Welcome to user mode! Hope you guys have some fun!\n");
    int ccc = 0, user_S_ans = 0, score = 0, keep_answer_right = 0;
    for (ptr = first; ptr != NULL && ccc < 10; ptr = ptr->linklist_next) {
        if (ptr->que.qid == id_choose[ccc]) {
            printf("\nQuestion id: %d\n", ptr->que.qid);
            printf("Question: %s\n",ptr->que.q_content);
            //printf("%d", ptr->que.ans);
            printf("How many people answered the question: %d\n", ptr->que.answered_num);
            printf("Correct percent: %.2f\n", ptr->que.correct_percent);
            printf("How many people got it right: %d\n", ptr->que.correct_num);

            printf("Now type your answer: ");
            scanf("%d", &user_S_ans);
            if (user_S_ans != ptr->que.ans) {
                printf("What a pity! You got it wrong!\n");
                keep_answer_right = 0;
                ptr->que.correct_percent = ((ptr->que.correct_num * 100) / ((ptr->que.answered_num) + 1));
            } else {
                printf("Amazing! You got the question right!\n");
                keep_answer_right++;
                if (keep_answer_right >= 2)
                    printf("Wow! You answered %d questions in a row!\n", keep_answer_right);
                score += 10;
                ptr->que.correct_num++;

                ptr->que.correct_percent = ((ptr->que.correct_num * 100) / ((ptr->que.answered_num) + 1));
                printf("debug: %.2f\n", ptr->que.correct_percent);
            }
            ptr->que.answered_num++;
            ccc++;
        }
    }
    printf("\nCongratulations! You got %d points!\n", score);


    fclose(fptr);

    //renew some info
    FILE *fptr_out;
    fptr_out = fopen("test.txt", "w");
    if (fptr_out == NULL) {
        printf("Failed to open file for writing.\n");
        return 0;
    }

    for (ptr = first; ptr != NULL && ptr->que.qid != 0; ptr = ptr->linklist_next) { //there some bug that will fprintf id = 0
        fprintf(fptr_out, "%d\n", ptr->que.qid);
        fputs(ptr->que.q_content, fptr_out);
        fprintf(fptr_out, "\n%d\n", ptr->que.ans);

        // 寫入更改後的 answered_num 和 correct_num
        fprintf(fptr_out, "%.2f\n", ptr->que.correct_percent);
        fprintf(fptr_out, "%d\n", ptr->que.answered_num);
        fprintf(fptr_out, "%d\n", ptr->que.correct_num);
    }

    //free memories and close files
    free(id_choose);
    ptr = first;
    while (ptr != NULL) {
        struct node* temp = ptr;
        ptr = ptr->linklist_next;
        free(ptr);
    }

    fclose(fptr_out);

    return 0;
}

char* qid_rand_choose(void) {    //return the pointer point to array which record the q_id been choosen
    srand( time(NULL) );
    int* record = malloc(10 * sizeof(int));//int record[10] = {0,0};
    int i , j;
    int max = 20;//int max = max_id();
    for (i = 1; i <= 10; i++) {
        if (max <10) {
            printf("The number of questions is not enough!(< 10)\n");
            return NULL;
        }
        do {
            record[i - 1] = rand() % (max - 1 + 1) + 1;
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

void Bubble_sort(int arr[], int num) {
    int temp;
    int flag = 0;
    for (int i = 1; (i < num) && (flag != 1); i++) { //i:the total run times
        flag = 1;
        for (int j = 0; j < num - i; j++) {       //after exchanging, the last one must be the biggest
            if (arr[j] > arr[j+1]) {  //num - i:the neighbors exchange times need
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                flag = 0;
            }
        }
    }
}
