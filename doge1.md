hello world by 408210054

typedef struct ques{

    int qid; //question_id;
　　char ques[//len]="";
　　int ans;
　　float correct_percent;
    int ask_num;
    int correct_num;

    //private
    ques* lchild_qid,rchild_qid;
    ques* linklist_frount,linklist_back;
    ques* lchild_correct_percent,rchild_correct_percent;
    
}ques;