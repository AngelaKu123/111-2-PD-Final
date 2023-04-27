#include<stdio.h>
#include<stdlib.h>

#define len 500

void main(){
    struct ques{

        //public
        int qid; //question_id;
        char ques[len];
        int ans;
        float correct_percent;
        int ask_num;
        int correct_num;

    

    };
    struct node{
        struct ques;

        struct ques* lchild_qid,rchild_qid;
        struct ques* linklist_frount,linklist_back;
        struct ques* lchild_correct_percent,rchild_correct_percent;

    };

    return;
}

