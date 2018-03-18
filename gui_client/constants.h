#ifndef CONNECTIONERROR_H
#define CONNECTIONERROR_H

#define CONN_OK 1
#define CONN_NOT_FOUND 2
#define CONN_ERROR 3
#define WINDOW_WIDTH 1000
#define GET_STUDENT_LIST 5
#define ADD_STUDENT 6
#define REMOVE_STUDENT 7
#define ADD_EXAM 8
#define GET_EXAM_LIST 9
#define START_EXAM_FOR_STUDENT 10
#define FORCED_EXAM_FINISH 11
#define ADD_QUESTION 12
#define REMOVE_QUESTION 12
#define GET_QUESTION_LIST 12
#define NOP 13
#define SUCCESS 14
#define FAIL 15
#define GET_SUBJECT_LIST 16
#define ADD_SUBJECT 17
#define REMOVE_SUBJECT 18
#define FILTER_FIRST_NAME 1
#define FILTER_MIDDLE_NAME 2
#define FILTER_LAST_NAME 3
#define FILTER_STUD_TICKET 4
#define FILTER_STUD_GROUP 5
#define SELECT_QUESTION_TYPE 1  //выбор одного или нескольких вариантов
#define INPUT_QUESTION_TYPE 2   //ввод ответа с клавиатуры
#define SEQUENCE_QUESTION_TYPE 3//выбор последовательности действий
#define MATCH_QUESTION_TYPE 4   //выбор группы
#endif // CONNECTIONERROR_H
