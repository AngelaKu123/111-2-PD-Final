//this is a database for question game

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"database.h"
#define len 500
#define INT_MAX 2147483647

#define getid -2100483647

//public funtion

//回傳qid對應的ques，若找不到(超出範圍or空洞)返回NULL
struct ques* search_ID_ques(int id);

int max_id();//回傳目前最大的qid，作為search獲取題目的取值範圍

//output all question with keyword,if keyword is empty string,will output all question
void keyword_output(char* keyword);

//output all question with correct_percent,from high to low
void cp_output();

void insert_newques(char* problem,int ans,float correct_percent,int answered_num,int corrent_num);

//delete the question by qid
void delete_ques(int id);

//return the number of questions;
int num_question();



//private funtion
struct node* search_ID_node(struct node* root,int id);
void deep_copy_ques(struct ques* des,struct ques* sor);
struct node* insert_node_qid(struct node* root_qid,struct node* inserted_node);
struct node* create_node(char* problem,int ans,float correct_percent,int answered_num,int corrent_num);
int getheight_qid(struct node* node);
int getBalanceFactor_qid(struct node* node);
struct node* update_qid(struct node* node);
struct node* rotateright_qid(struct node* node);
struct node* rotateleft_qid(struct node* node);
struct node* findPredecessor_qid(struct node* node);
struct node* balance_qid(struct node* root);
struct node* delete_node_qid(struct node* root,int qid);

void keyword_pri(char* keyword,struct node* root);

//return the num of questions,should equal or less than max_id()
int num_ques(struct node* root);

int id_factor(int value);

int compfunc(const void* a, const void* b);

void insert_in_arr(struct node** arr,struct node* root);





struct link_node{
    int value;
    struct link_node *next;
};

static struct node *root_qid=NULL;

void insert_newques(char* problem,int ans,float correct_percent,int answered_num,int corrent_num){
    if(NULL==root_qid){ // in this case,tree_qid is empty tree.
        struct node* newnode=create_node(problem,ans,correct_percent,answered_num,corrent_num);
        root_qid=insert_node_qid(root_qid,newnode);
        return;
    }
    else{
        struct node* temp=create_node(problem,ans,correct_percent,answered_num,corrent_num);
        root_qid=insert_node_qid(root_qid,temp);
    }
}

struct node* create_node(char* problem,int ans,float correct_percent,int answered_num,int corrent_num){
    int newID=id_factor(getid);
    struct node* temp_node=malloc(sizeof(struct node));
    strcpy(temp_node->que.q_content,problem);
    temp_node->que.ans=ans;
    temp_node->que.answered_num=answered_num;
    temp_node->que.correct_num=corrent_num;
    temp_node->que.correct_percent=correct_percent;
    temp_node->que.qid=newID;

    temp_node->height_qid=1;
    temp_node->lchild_qid=NULL;
    temp_node->rchild_qid=NULL;
    
    
    return temp_node;
}

struct node* insert_node_qid(struct node* root_qid,struct node* inserted_node){
    if(NULL == root_qid){
        update_qid(inserted_node);
        return inserted_node;
    }
    if(inserted_node->que.qid < root_qid->que.qid){
        root_qid->lchild_qid = insert_node_qid(root_qid->lchild_qid,inserted_node);
    }
    else if(inserted_node->que.qid > root_qid->que.qid){
        root_qid->rchild_qid = insert_node_qid(root_qid->rchild_qid,inserted_node);
    }
    else{
        printf("error:already have qid:%d\n",inserted_node->que.qid);
        return root_qid;
    }

    return balance_qid(root_qid);
}

struct ques* search_ID_ques(int id){
    struct node* target_node=search_ID_node(root_qid,id);
    if(NULL==target_node)return NULL;
    // struct ques* target_que=malloc(sizeof(struct ques));
    // deep_copy_ques(target_que,&(target_node->que));
    return &target_node->que;
}

struct node* search_ID_node(struct node* root,int id){
    if(NULL==root){
        return NULL;
    }
    if(root->que.qid==id){
        return root;
    }
    else if(root->que.qid<id){
        return search_ID_node(root->rchild_qid,id);
    }
    else if(root->que.qid>id){
        return search_ID_node(root->lchild_qid,id);
    }
    else{
        printf("error_2\n");
        return NULL;
    }

}

void deep_copy_ques(struct ques* des,struct ques* sor){
    
    des->answered_num=sor->answered_num;
    des->ans=sor->ans;
    des->correct_num=sor->correct_num;
    des->correct_percent=sor->correct_percent;
    des->qid=sor->qid;
    strcpy(des->q_content,sor->q_content);
}

void delete_ques(int id){
    struct node* deleted_node=search_ID_node(root_qid,id);
    if(NULL == deleted_node){
        printf("error: the id is not exist,can't delete.\n");
        return;
    }
    root_qid = delete_node_qid(root_qid,id);
    id_factor(id);
}

struct node* delete_node_qid(struct node* root,int deleted_id){
    if(NULL == root){
        return root;
    }
    if(deleted_id < root->que.qid){
        root->lchild_qid = delete_node_qid(root->lchild_qid,deleted_id);
    }
    else if(deleted_id > root->que.qid){
        root->rchild_qid = delete_node_qid(root->rchild_qid,deleted_id);
    }
    else {
        if(root->lchild_qid == NULL || root->rchild_qid == NULL){
            struct node* temp = root->lchild_qid ? root->lchild_qid : root->rchild_qid;
            if(NULL == temp){
                temp=root;
                root=NULL;
            }
            else{
                //*root=*temp;
                memcpy(root,temp,sizeof(struct node));
            }
            free(temp);
        }
        else{
            struct node* predecessor=findPredecessor_qid(root);
            root->que=predecessor->que;
            root->lchild_qid=delete_node_qid(root->lchild_qid,predecessor->que.qid);
        }
    }

    if(NULL == root)return NULL;
    return balance_qid(root);
}


struct node* update_qid(struct node* node){
    int l_h,r_h;
    l_h=getheight_qid(node->lchild_qid);
    r_h=getheight_qid(node->rchild_qid);
    node->height_qid= l_h>r_h ? (l_h+1):(r_h+1);
}

struct node* balance_qid(struct node* root){
    update_qid(root);
    int balanceFactor = getBalanceFactor_qid(root);
    if(balanceFactor > 1){
        if(getBalanceFactor_qid(root->lchild_qid) < 0){
            root->lchild_qid = rotateleft_qid(root->lchild_qid);
        }
        return rotateright_qid(root);
    }
    if(balanceFactor < -1){
        if(getBalanceFactor_qid(root->rchild_qid) > 0){
            root->rchild_qid=rotateright_qid(root->rchild_qid);
        }
        return rotateleft_qid(root);
    }
    return root;
}

int getheight_qid(struct node* node){
    if(NULL == node){
        return 0;
    }
    else return node->height_qid;
}

int getBalanceFactor_qid(struct node* node){
    if(NULL == node)return 0;
    else return getheight_qid(node->lchild_qid)-getheight_qid(node->rchild_qid);

}

struct node* rotateright_qid(struct node* node){
    struct node* newroot=node->lchild_qid;
    node->lchild_qid = newroot->rchild_qid;
    newroot->rchild_qid=node;
    update_qid(node);
    update_qid(newroot);
    return newroot;

}

struct node* rotateleft_qid(struct node* node){
    struct node* newroot=node->rchild_qid;
    node->rchild_qid=newroot->lchild_qid;
    newroot->lchild_qid=node;
    update_qid(node);
    update_qid(newroot);

    return newroot;
}

struct node* findPredecessor_qid(struct node* node){
    struct node* current=node->lchild_qid;
    while(NULL != current->rchild_qid){
        current=current->rchild_qid;
    }
    return current;
}

void exchange_node_value(struct node* n1,struct node* n2){
    struct ques temp=n1->que;
    n1->que=n2->que;
    n2->que=temp;
}

void keyword_output(char* keyword){
    keyword_pri(keyword,root_qid);
}

int max_id(){
    struct node* current=root_qid;
    while(current->rchild_qid!=NULL){
        current=current->rchild_qid;
    }
    return current->que.qid;
}

int num_ques(struct node* root){
    if(NULL == root) return 0;
    return 1+num_ques(root->lchild_qid)+num_ques(root->rchild_qid);
}

void keyword_pri(char* keyword,struct node* root){
    if(NULL == root)return;
    keyword_pri(keyword,root->lchild_qid);
    if(NULL != strstr(root->que.q_content,keyword)){
        printf("%d.%s\n",root->que.qid,root->que.q_content);
    }
    
    keyword_pri(keyword,root->rchild_qid);
}

int id_factor(int value){
    static int max_id=0;
    static struct link_node* head=NULL;

    if(value>0){//input the idle ID
        struct link_node* newnode=malloc(sizeof(struct link_node));
        if(NULL == newnode){
            printf("error: function %s is memory allocation failed.\n",__func__);
            return -1;
        }
        newnode->value=value;
        newnode->next=head;
        head=newnode;
    }

    else if(value == getid){//allocate id 
        if(head != NULL){ //stack have idle id
            struct link_node* popnode=head;
            head=head->next;
            int popvalue=popnode->value;
            free(popnode);
            return popvalue;
        }
        else{ //stack is empty
            if(max_id == INT_MAX){
                printf("error:question_id is overflow.\n");
                return -1;
            }
            max_id++;
            return max_id;
        }
    }
    else{
        printf("error:in function %s have invalid input.\n",__func__);
        return -1;
    }
}

int compfunc(const void* a, const void* b){
    const struct node* n1 = (const struct node*)a;
    const struct node* n2 = (const struct node*)b;
    if(n1->que.correct_percent <= n2->que.correct_percent) return -1 ;
    else return 1;
}

void cp_output(){
    int question_num=num_ques(root_qid);
    struct node* arr[question_num];
    insert_in_arr(arr,root_qid);
    for(int i=0;i<question_num;i++){
        for( int j=0;j<question_num-i-1;j++){
            if(arr[j]->que.correct_percent > arr[j+1]->que.correct_percent){
                struct node* temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
    for(int i=0;i<question_num;i++){
        printf("correct rate:%.2f,content:%s\n",arr[i]->que.correct_percent*100,arr[i]->que.q_content);
    }
}

void insert_in_arr(struct node** arr,struct node* root){
    if(NULL == root)return;
    static int counter=0;
    arr[counter]=root;
    counter++;
    insert_in_arr(arr,root->lchild_qid);
    insert_in_arr(arr,root->rchild_qid);
}

int num_question(){
    return num_ques(root_qid);
}