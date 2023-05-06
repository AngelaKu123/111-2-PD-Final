#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define len 500


struct ques* search_ID_ques(int id);//need free() the return
struct node* search_ID_node(struct node* root,int id);
struct ques* search_keyword(char *keyword);



void deep_copy_ques(struct ques* des,struct ques* sor);

void insert_newques(char* problem,int ans,int correct_percent,int answered_num,int corrent_num);

struct node* insert_node_qid(struct node* root_qid,struct node* inserted_node);
struct node* insert_node_correct_percent(struct node* root_correct_percent ,struct node* inserted_node);

void delete_ques(int id);//delete the question by qid

struct node* create_node(char* problem,int ans,int correct_percent,int answered_num,int corrent_num);

struct ques{

//public
    int qid; //question_id;
    char q_content[len];
    int ans;
    float correct_percent;
    int answered_num;
    int correct_num;
};



struct node{
    struct ques que;

    struct node* linklist_next,*linklist_prior;
    struct node* lchild_qid,*rchild_qid,*parent_qid;
    struct node* lchild_correct_percent,*rchild_correct_percent,*parent_correct_percent;
};

static struct node *root_qid=NULL,*root_correct_percent=NULL;
static struct node *head=NULL,*rear=NULL;
void main(){
    
    insert_newques("doge",1,0.5,2,1);
    insert_newques("sun is black.",0,0.7,10,7);
    insert_newques("professor is noob.",1,1.0,100,100);
    struct ques* test=search_ID_ques(3);
    printf("%d.%s\n",test->qid,test->q_content);
    test=search_ID_ques(1);
    printf("%d.%s\n",test->qid,test->q_content);
    return;
}

void insert_newques(char* problem,int ans,int correct_percent,int answered_num,int corrent_num){
    if(NULL==root_qid){
        if(NULL!=root_correct_percent){printf("error_1\n");return;}
        root_qid=create_node(problem,ans,correct_percent,answered_num,corrent_num);
        root_correct_percent=root_qid;
        rear=root_qid;
        head=root_qid;
        return;
    }
    else{
        struct node* temp=create_node(problem,ans,correct_percent,answered_num,corrent_num);
        rear->linklist_next=temp;
        temp->linklist_prior=rear;
        rear=temp;
        insert_node_qid(root_qid,temp);
        insert_node_correct_percent(root_correct_percent,temp);
    }
}

struct node* create_node(char* problem,int ans,int correct_percent,int answered_num,int corrent_num){
    static int max_qid=0;
    max_qid++;
    struct node* temp_node=malloc(sizeof(struct node));
    strcpy(temp_node->que.q_content,problem);
    temp_node->que.ans=ans;
    temp_node->que.answered_num=answered_num;
    temp_node->que.correct_num=corrent_num;
    temp_node->que.correct_percent=correct_percent;
    temp_node->que.qid=max_qid;

    temp_node->lchild_correct_percent=NULL;
    temp_node->lchild_qid=NULL;
    temp_node->linklist_next=NULL;
    temp_node->linklist_prior=NULL;
    temp_node->parent_correct_percent=NULL;
    temp_node->parent_qid=NULL;
    temp_node->rchild_correct_percent=NULL;
    temp_node->rchild_qid=NULL;

    return temp_node;
}

struct node* insert_node_qid(struct node* root_qid,struct node* inserted_node){
    if(NULL==root_qid){
        root_qid=inserted_node;
        return root_qid;
    }
    
    if(inserted_node->que.qid<=root_qid->que.qid){
        root_qid->lchild_qid=insert_node_qid(root_qid->lchild_qid,inserted_node);
        root_qid->lchild_qid->parent_qid=root_qid;
    }
    else if(inserted_node->que.qid>root_qid->que.qid){
        root_qid->rchild_qid=insert_node_qid(root_qid->rchild_qid,inserted_node);
        root_qid->rchild_qid->parent_qid=root_qid;
    }

    return root_qid;
}

struct node* insert_node_correct_percent(struct node* root_correct_percent ,struct node* inserted_node){
    if(NULL==root_correct_percent){
        root_correct_percent=inserted_node;
        return root_correct_percent;
    }
    
    if(inserted_node->que.correct_percent<=root_correct_percent->que.correct_percent){
        root_correct_percent->lchild_correct_percent=insert_node_correct_percent(root_correct_percent->lchild_correct_percent,inserted_node);
        root_correct_percent->lchild_correct_percent->parent_correct_percent=root_correct_percent;
    }
    else if(inserted_node->que.correct_percent>root_correct_percent->que.correct_percent){
        root_correct_percent->rchild_correct_percent=insert_node_correct_percent(root_correct_percent->rchild_correct_percent,inserted_node);
        root_correct_percent->rchild_correct_percent->parent_correct_percent=root_correct_percent;
    }

    return root_correct_percent;
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