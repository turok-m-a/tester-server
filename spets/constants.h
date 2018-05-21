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
#define REMOVE_QUESTION 21
#define GET_QUESTION_LIST 22
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
#define FILTER_LOGIN 6
#define SELECT_QUESTION_TYPE 1  //выбор одного или нескольких вариантов
#define INPUT_QUESTION_TYPE 2   //ввод ответа с клавиатуры
#define SEQUENCE_QUESTION_TYPE 3//выбор последовательности действий
#define MATCH_QUESTION_TYPE 4   //выбор группы
#define EDIT_SUBJECT_LIST_FOR_QUESTION 19
#define SUBJECT_LIST_FOR_QUESTION_ADD 20
#define SUBJECT_LIST_FOR_QUESTION_REMOVE 24
#define DELETE_EXAM 25
#define SET_EXAM_TIME 26
#define GET_PASS_STATE 27
#define VIEW_REPORT 28
#define ADD_USER 29
#define DEL_USER 30
#define FIND_USER 31
#define RESET_PASSWORD 32
#define USER_TEACHER 1
#define USER_ADMIN 2

#define DB_HOST "localhost"
#define DB_NAME "tester"
#define DB_USER "tester"
#define DB_PASSWORD "12345"
#endif // CONNECTIONERROR_H