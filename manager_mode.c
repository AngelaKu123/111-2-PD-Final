/*  管理者模式相關函數
    判斷使用者輸入是否正確(抓輸入資料，跟資料庫資料比較)
    計算某題目不同次被回答的答對率
    讓管理者增減題目的函數>>database裡面有了

    ????????
    設計排行榜
    分數計算
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

#if !include_database
#define len 500
struct ques{
    //public
    int qid; //question_id;
    char q_content[len];
    int ans;
    float correct_percent;
    int answered_num;
    int correct_num;
};
#endif



void interactive_place(void);
static struct ques* search_question_interactive(void);
static void display_question_infomation(struct ques* ques_to_display);
static void edit_question_infomation(struct ques *question_to_edit);
int check_answer(int ques_id, int player_answer);
void modify_question_correctness_percent(int operation, int id, struct ques* ques_id);



void interactive_place(void){
    printf("Welcome to manager mode!\n");
    
    #if identification_system
    printf("Please enter the account and password.\n");
    char account[100], password[100];// but less than 21
    printf("account: ");
    scanf("%s", account);
    printf("password: ");
    scanf("%s", password);
    #endif

    while(1){
        int opr=0;
        while(opr<1 || 3<opr){
            printf("Choose one operation below:\n"
                "1. Add a new question\n"
                "2. Display, Edit, or Delete a certain question's information\n"// use id or keyword to search
                "3. Exit\n");
            scanf("%d", &opr);
        }


        if(opr==3){
            break;
        }
        else if(opr==1){
            // "1. Add a new question\n"
            char tmp_q_content[len];
            int tmp_ans=0;
            printf("Enter the question content within %d word, "
                "and the answer should be a integer", len-1);

            printf("question: ");
            fgets(tmp_q_content, len-1, stdin);// input until \n
            printf("answer: ");
            scanf("%d", &tmp_ans);

            insert_newques(tmp_q_content, tmp_ans, 0, 0, 0);
        }
        else if(opr==2){
            // "2. Display, Edit, or Delete a certain question's information\n"// use id or keyword to search

            //search
            struct ques *search_result=NULL;
            search_result=search_question_interactive();

            // display the search results
            display_question_infomation(search_result);

            // ask edit or delete or leave
            edit_question_infomation(search_result);

            /*  the search function in database
                return a piece of new memory store the info,
                so need to free before the pointer disappear
                as the program exit this block    */ 
            free(search_result);
        }
    }
    return;
}



// use the function indatabase to search id or keyword
static struct ques* search_question_interactive(void){
    //search
    int search_way=0;
    struct ques *question_search_result=NULL;
    while(search_way!=1 || search_way!=2){
        printf("Search question by:\n"
            "1. question ID\n"
            "2. keyword\n");
        scanf("%d", &search_way);
    }

    int tmp_id_for_search=-1;
    char tmp_keyword_for_search[len];
    if(search_way==1){
        printf("Please enter the question ID: ");
        scanf("%d", &tmp_id_for_search);
        question_search_result=search_ID_ques(tmp_id_for_search);
    }
    else{
        printf("Please enter the keyword: ");
        fgets(tmp_keyword_for_search, len-1, stdin);// input keyword
        question_search_result=search_keyword(tmp_keyword_for_search);
    }

    return question_search_result;
}



static void display_question_infomation(struct ques* ques_to_display){
    if(ques_to_display==NULL){
        printf("This question doesn't exist.\n");
    }
    else{
        printf("The information of ID: %d question is below\n", ques_to_display->qid);
        printf("question: %s\n", ques_to_display->q_content);
        printf("answer: %d\n", ques_to_display->ans);
        printf("This question has been totally answered %d times,\n", ques_to_display->answered_num);
        printf("%d of all was correct,\n", ques_to_display->correct_num);
        printf("and the correct rate was %f\n", ques_to_display->correct_percent);
    }
    return;
}



static void edit_question_infomation(struct ques *question_to_edit){
    while(1){
        int edit_choice=0;
        while(edit_choice<1 || edit_choice>7){
            printf("\n\n================================\n\n"
            "What do you want to do?\n"
            "1. Edit the question id\n"
            "2. Edit the question\n"
            "3. Edit the answer\n"
            "4. Edit the answer times\n"
            "5. Edit the correct times\n"
            "6. Delete this question\n"
            "7. Back\n");
            scanf("%d", &edit_choice);
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //                                                                                            //
        //                                                                                            //
        //  There id the same problem as the modify_question_correctness_percent() function below     //
        //  it can't touch the original memory stored the question, so the info can't be changed      //
        //                                                                                            //
        //                                                                                            //
        ////////////////////////////////////////////////////////////////////////////////////////////////
        if(edit_choice==7){
            break;
        }
        else if(edit_choice==1){
            printf("Enter new question ID: ");
            scanf("%d", &question_to_edit->qid);
        }
        else if(edit_choice==2){
            printf("Enter new question content: ");
            fgets(question_to_edit->q_content, len-1, stdin);// input until \n
        }
        else if(edit_choice==3){
            printf("Enter new answer: ");
            scanf("%d", &question_to_edit->ans);
        }
        else if(edit_choice==4){
            printf("Enter new answer times: ");
            scanf("%d", &question_to_edit->answered_num);
        }
        else if(edit_choice==5){
            printf("Enter new correct times: ");
            scanf("%d", &question_to_edit->correct_num);
        }
        else if(edit_choice==6){
            char tmp='a';
            while(tmp != 'Y' || tmp != 'N'){
                printf("Are you sure to DELETE this question(Y/N): ");
                scanf("%c", &tmp);
            }
            if(tmp=='Y'){
                delete_ques(question_to_edit->qid);// function in database
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
        // tmp_ques need to free
        free(tmp_ques);
        return 1;
    }
    else{
        free(tmp_ques);
        return 0;
    }
}



/*  use to recalculate correct rate after each answer
    reset: 0
    recalculate: 1,correct answer+1, 2,error answer+1
*/
void modify_question_correctness_percent(int operation, int id, struct ques* ques_id){
    struct ques* tmp_ques=search_ID_ques(ques_id);// function in database
    // search id return a copy not the origin one
////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                                                                    //
//    need to access to the original question memory in order to modify its value     //
//                                                                                    //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
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