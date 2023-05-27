//this is a database for question game

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define len 500


//public funtion
//need free() the return
struct ques* search_ID_ques(int id);

struct ques_set* search_keyword(char *keyword);

//output all question with keyword,if keyword is empty string,will output all question
void keyword_output(char* keyword);

void insert_newques(char* problem,int ans,float correct_percent,int answered_num,int corrent_num);

//delete the question by qid
void delete_ques(int id);



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
void inorder_trav(struct node* root);

//structure
struct ques{ 

    int qid; //question_id;
    char q_content[len];
    int ans;
    float correct_percent;
    int answered_num;
    int correct_num;
};

struct node{
    struct ques que;

    int height_qid; //for a leaf node,height = 1.

    struct node* linklist_next,*linklist_prior;
    struct node* lchild_qid,*rchild_qid;
};

struct ques_set{ //for retrun question more than one
    int num_ques;
    struct ques* question_set[];
};

static struct node *root_qid=NULL;
static struct node *head=NULL,*rear=NULL;

void main(){
    // insert_newques("problem 1",1,0.5,2,1);
    // insert_newques("problem 2",0,0.7,10,7);
    // insert_newques("problem 3",1,1.0,100,100);
    // insert_newques("problem 4",1,1.0,100,100);
    // insert_newques("problem 5",1,1.0,100,100);
    // insert_newques("problem 6",1,1.0,100,100);
    // insert_newques("problem 7",1,1.0,100,100);
    // insert_newques("problem 8",1,1.0,100,100);
    // struct ques* test=search_ID_ques(3);
    // printf("%d.%s\n",test->qid,test->q_content);
    //test=search_ID_ques(1);
    // printf("%d.%s\n",test->qid,test->q_content);
    // printf("height:%d\n",root_qid->height_qid);
    // printf("%d.%s\n",root_qid->que.qid,root_qid->que.q_content);
    // return;

    for(int i=1;i<=32;i++){
        char string[len];
        sprintf(string,"problem %d",i);
        insert_newques(string,i,1.0,5,5);
    }
    keyword_output("");
}

void insert_newques(char* problem,int ans,float correct_percent,int answered_num,int corrent_num){
    if(NULL==root_qid){ // in this case,tree_qid is empty tree.
        struct node* newnode=create_node(problem,ans,correct_percent,answered_num,corrent_num);
        root_qid=insert_node_qid(root_qid,newnode);
        rear=root_qid;
        head=root_qid;
        return;
    }
    else{
        struct node* temp=create_node(problem,ans,correct_percent,answered_num,corrent_num);
        rear->linklist_next=temp;
        temp->linklist_prior=rear;
        rear=temp;
        root_qid=insert_node_qid(root_qid,temp);
    }
}

struct node* create_node(char* problem,int ans,float correct_percent,int answered_num,int corrent_num){
    static int max_qid=0;
    max_qid++;
    struct node* temp_node=malloc(sizeof(struct node));
    strcpy(temp_node->que.q_content,problem);
    temp_node->que.ans=ans;
    temp_node->que.answered_num=answered_num;
    temp_node->que.correct_num=corrent_num;
    temp_node->que.correct_percent=correct_percent;
    temp_node->que.qid=max_qid;

    temp_node->height_qid=1;
    temp_node->lchild_qid=NULL;
    temp_node->rchild_qid=NULL;
    temp_node->linklist_next=NULL;
    temp_node->linklist_prior=NULL;
    
    
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
        printf("already have qid:%d\n",inserted_node->que.qid);
        return root_qid;
    }

    return balance_qid(root_qid);
}

struct ques* search_ID_ques(int id){
    struct node* target_node=search_ID_node(root_qid,id);
    if(NULL==target_node)return NULL;
    struct ques* target_que=malloc(sizeof(struct ques));
    deep_copy_ques(target_que,&(target_node->que));
    return target_que;
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

    deleted_node->linklist_next->linklist_prior=deleted_node->linklist_prior;
    deleted_node->linklist_prior->linklist_next=deleted_node->linklist_next;
    delete_node_qid(root_qid,id);
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
                *root=*temp;
            }
            free(temp);
        }
        else{
            struct node* predecessor=findPredecessor_qid(root);
            root->que=predecessor->que;
            root->lchild_qid=delete_node_qid(root->lchild_qid,predecessor->que.qid);
        }
    }

    if(NULL == root)return root;
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
    struct node* current=head;
    while(NULL != current){
        if(NULL != strstr(current->que.q_content,keyword)){
            printf("%d:%s\n",current->que.qid,current->que.q_content);
        }
        current=current->linklist_next;
    }

}

void inorder_trav(struct node* root){
    if(NULL==root)return;
    inorder_trav(root->lchild_qid);
    printf("%d:%s\n",root->que.qid,root->que.q_content);
    inorder_trav(root->rchild_qid);
}