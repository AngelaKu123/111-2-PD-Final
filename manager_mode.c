/*  管理者模式的函數
    1. 新增題目
    2. 查詢現有題目
    3. 編輯題目或刪除

    其他函數
    1. 判斷使用者輸入是否正確
    2. 計算某題目不同次被回答的答對率(每次回答後簡易重新計算)

    有空再做
    1. 依照題目的正確率、回答次數等作排行榜
    2. 如果時間足夠，要新增身分系統(玩家帳號、密碼、是否為管理員、玩家資料)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manager_mode.h"
#include "database.h"


// #ifndef DATABASE_H
/*  for temporary using */
// #define len 500
// struct ques{
//     //public
//     int qid; //question_id;
//     char q_content[len];
//     int ans;
//     float correct_percent;
//     int answered_num;
//     int correct_num;
// };
// #endif



void manager_mode(void);
static void search_question_interactive(void);
static void display_question_infomation(struct ques* ques_to_display);
static void edit_question_infomation(void);
int check_answer(int ques_id, int player_answer);
void modify_question_correctness_percent(int operation, int id);



void manager_mode(void){
    printf("Welcome to manager mode!\n");

    // identification
    char password[100];// but less than 21
    printf("Please enter password: ");
    scanf("%s", password);// input until \n

    if(strcmp(password, "1"/*    the correct password is set here    */) == 0){
        printf("Correct!\n");
    }
    else{
        printf("Error!\n");
        return;
    }


    while(1){
        printf("\n================================\n\n");
        int opr=0;
        while(opr<1 || 5<opr){
            printf("Choose one operation below:\n"
                "1. Add a new question\n"
                "2. Display a certain question's information\n"// use id or keyword to search
                "3. Display all questions\n"
                "4. Edit or Delete a certain question's information\n"
                "5. Exit\n"
                "Enter: ");
            scanf("%d", &opr);
        }
        printf("\n================================\n\n");
        if(opr==5){
            return;
        }
        else if(opr==1){
            // "1. Add a new question\n"
            char tmp_q_content[len];
            int tmp_ans=0;
            printf("Enter the question content within %d characters, "
                "and the answer should be an integer\n", len-1);
            printf("question (use EOF to terminate input):\n");

            int tmp_len=0;
            while(fgets(tmp_q_content+tmp_len, len-1-tmp_len, stdin) != NULL)   tmp_len=strlen(tmp_q_content);
            if(tmp_len==499) printf("The length of the question has reach %d characters\n", len-1);
            if(tmp_q_content[strlen(tmp_q_content)-1] == '\n'){
                tmp_q_content[strlen(tmp_q_content)-1]='\0';
            }

            printf("answer (an integer): ");
            scanf("%d", &tmp_ans);

            insert_newques(tmp_q_content, tmp_ans, 0, 0, 0);
        }
        else if(opr==2){
            // "2. Display, Edit, or Delete a certain question's information\n"// use id or keyword to search

            //search and display
            search_question_interactive();
        }
        else if(opr==3){
            keyword_output("");
        }
        else if(opr==4){
            // "3. Edit or Delete a certain question's information\n"
            edit_question_infomation();
        }
    }
    return;
}



// use the function in database to search id or keyword
static void search_question_interactive(void){
    //search
    int search_way=0;
    struct ques *question_search_result=NULL;
    while(search_way!=1 && search_way!=2){
        printf("Search question by:\n"
            "1. question ID\n"
            "2. keyword\n"
            "Enter: ");
        scanf(" %d", &search_way);
    }

    printf("\n================================\n\n");

    int tmp_id_for_search=-1;
    char tmp_keyword_for_search[len];
    if(search_way==1){
        printf("Please enter the question ID: ");
        scanf("%d", &tmp_id_for_search);
        question_search_result=search_ID_ques(tmp_id_for_search);
        // display the search results
        display_question_infomation(question_search_result);
    }
    else{
        printf("Please enter the keyword (use EOF to terminate input):\n");

        int tmp_len=0;
        while(fgets(tmp_keyword_for_search+tmp_len, len-1-tmp_len, stdin) != NULL)      tmp_len=strlen(tmp_keyword_for_search);
        if(tmp_len==499) printf("The length of the question has reach %d characters\n", len-1);
        if(tmp_keyword_for_search[strlen(tmp_keyword_for_search)-1] == '\n'){
            tmp_keyword_for_search[strlen(tmp_keyword_for_search)-1]='\0';
        }

        // printf("'%s'", tmp_keyword_for_search);
        // exit(EXIT_SUCCESS);
        // question_search_result=search_keyword(tmp_keyword_for_search);
        keyword_output(tmp_keyword_for_search);
    }

    return;
}



static void display_question_infomation(struct ques* ques_to_display){
    if(ques_to_display==NULL){
        printf("This question doesn't exist.\n");
    }
    else{
        printf("The information is below\n");
        printf("\n---------------------------------\n");
        printf("question ID: %d\n", ques_to_display->qid);
        printf("question: %s\n\n", ques_to_display->q_content);
        printf("answer: %d\n\n", ques_to_display->ans);
        printf("This question has been totally answered %d times, ", ques_to_display->answered_num);
        printf("%d of all was correct, ", ques_to_display->correct_num);
        printf("and the correct rate was %f\n", ques_to_display->correct_percent);
        printf("---------------------------------\n");
    }
    return;
}



static void edit_question_infomation(void){
    while(1){
        // locate question
        int ques_id=-1;
        struct ques *question_to_edit=NULL;
        printf("Which question do you want to modify? (enter -1 to back)\n"
            "qusestion id: ");
        scanf("%d", &ques_id);
        if(ques_id==-1) return;
        question_to_edit=search_ID_ques(ques_id);

        printf("\n=========================================\n");
        printf("The original question information:\n\n");
        display_question_infomation(question_to_edit);
        printf("\n=========================================\n\n");
        if(question_to_edit == NULL)  return;

        // modify
        int edit_choice=0;
        while(edit_choice<1 || edit_choice>7){
            printf(
            "What do you want to do?\n"
            "1. Edit the question\n"
            "2. Edit the answer\n"
            "3. Edit the answer times\n"
            "4. Edit the correct times\n"
            "5. Delete this question\n"
            "6. Back\n");
            scanf("%d", &edit_choice);
        }

        if(edit_choice==6){
            break;
        }
        else if(edit_choice==1){
            printf("Enter new question content (use EOF to terminate input):\n");
            
            int tmp_len=0;
            while(fgets((question_to_edit->q_content)+tmp_len, len-1-tmp_len, stdin) != NULL)   tmp_len=strlen(question_to_edit->q_content);
            if(tmp_len==499) printf("The length of the question has reach %d characters\n", len-1);
            if(question_to_edit->q_content[strlen(question_to_edit->q_content)-1] == '\n'){
                question_to_edit->q_content[strlen(question_to_edit->q_content)-1]='\0';
            }
        }
        else if(edit_choice==2){
            printf("Enter new answer (an integer) : ");
            scanf("%d", &question_to_edit->ans);
        }
        else if(edit_choice==3){
            printf("Enter new answer times (an integer) : ");
            scanf("%d", &question_to_edit->answered_num);
        }
        else if(edit_choice==4){
            printf("Enter new correct times (an integer) : ");
            scanf("%d", &question_to_edit->correct_num);
        }
        else if(edit_choice==5){
            char tmp='a';
            while(tmp != 'Y' && tmp != 'N'){
                printf("Are you sure to DELETE this question(Y/N): ");
                scanf(" %c", &tmp);
            }
            if(tmp=='Y'){
                delete_ques(question_to_edit->qid);// function in database
                question_to_edit=NULL;
            }
        }
        printf("\n\n=========================================\n");
        printf("The updated question information:\n\n");
        display_question_infomation(question_to_edit);
        printf("\n=========================================\n\n");
    }
    return;
}



// use question_id to check the input answer
int check_answer(int ques_id, int player_answer){
    struct ques* tmp_ques=search_ID_ques(ques_id);// function in database

    if(player_answer==tmp_ques->ans){
        return 1;
    }
    else{
        return 0;
    }
}



/*  use to recalculate correct rate after each answer
    reset: 0
    recalculate: 1,correct answer+1, 2,error answer+1
*/
void modify_question_correctness_percent(int operation, int id){
    struct ques* tmp_ques=search_ID_ques(id);// function in database
    if(operation==0){
        tmp_ques->answered_num=0;
        tmp_ques->correct_num=0;
        tmp_ques->correct_percent=0;
        return;
    }
    else if(operation==1){
        tmp_ques->answered_num++;
        tmp_ques->correct_num++;
        tmp_ques->correct_percent= tmp_ques->correct_num / tmp_ques->answered_num;
        return;
    }
    else if(operation==2){
        tmp_ques->answered_num++;
        tmp_ques->correct_percent= tmp_ques->correct_num / tmp_ques->answered_num;
        return;
    }
    else{
        fprintf(stderr,"Error in \"%s\" function", __func__);
        return;
    }
}
