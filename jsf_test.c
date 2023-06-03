#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "database.h"

void main(){

    char string[100];

    for(int i=1;i<8;i++){
        sprintf(string,"problem %d",i);
        insert_newques(string,1,1.0/i,i,1);
    }

    for(int i=4;i<=4;i++){
        printf("we will delete problem %d:\n",i);
        delete_ques(i);
        keyword_output("");
        printf("we have %d question now.\n",num_question());
    }

    cp_output();



}