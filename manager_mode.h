#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

void interactive_place(void);
void search_question_interactive(void);
int check_answer(int ques_id, int player_answer);
void modify_question_correctness_percent(int operation, int id);