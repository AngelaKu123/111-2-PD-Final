#ifndef DATABASE_H
#define DATABASE_H

#define len 500
#define INT_MAX 2147483647

// Public functions
struct ques* search_ID_ques(int id);
int max_id();
void cp_output();
void keyword_output(char* keyword);
void insert_newques(char* problem, int ans, float correct_percent, int answered_num, int corrent_num);
void delete_ques(int id);
int num_question();

// Structures
struct ques {
    int qid;                 // Question ID
    char q_content[len];     // Question content
    int ans;                 // Answer
    float correct_percent;   // Correct percentage
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

#endif /* DATABASE_H */
