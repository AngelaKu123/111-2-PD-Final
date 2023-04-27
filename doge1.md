hello world by 408210054



typedef struct ques{

    //public
    int qid; //question_id;
　　char ques[//len]="";
　　int ans;
　　float correct_percent;
    int ask_num;
    int correct_num;

    

}ques;


struct node{
    struct ques;

    ques* lchild_qid,rchild_qid;
    ques* linklist_frount,linklist_back;
    ques* lchild_correct_percent,rchild_correct_percent;

}
