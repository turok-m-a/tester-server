#include "connectionthread.h"

void connectionThread::processStudent()
{
    dataBase & db = dataBase::getInstance();
    int subject_id,status,select_type,exam_id;
    QByteArray question_list;
    status = db.getStudentCurrentExamState(userId,subject_id,select_type,question_list,exam_id);   
    if (status != 11){
        int code = 2;//нет допуска
        send(sockDescriptor,(char*)&code,4,0);
        closesocket(sockDescriptor);
        return;
    }
    if (opCode == 2){
        processStudentAnswers();
    }
    if (select_type == 1) { //список вопросов заранее задан
        question_list = db.getQuestionsForExam(question_list); //получаем сами вопросы для передачи на клиент.
        int size = question_list.size();
        send(sockDescriptor,(char*)&size,sizeof(int),0);
        send(sockDescriptor,question_list.constData(),size,0);
    }
    //to-do: рандомные вопросы по предмету

}

void connectionThread::processStudentAnswers()
{

}

connectionThread::connectionThread(SOCKET s, QObject *parent) :
    QThread(parent), sockDescriptor(s),userType (0) {}

void connectionThread::run()
{
    cout << "thread start\n";
    if(!authorization()) {
        int code = 1;//студента с таким № билета нет
        send(sockDescriptor,(char*)&code,4,0);
        closesocket(sockDescriptor);
        return;
    }
    switch (userType) {
    case 1:
        processStudent();
        break;
    case 2:
        //processTeacher();
        break;
    default:
        break;
    }
}

bool connectionThread::authorization()
{
    recv(sockDescriptor,(char*)&userType,sizeof(int),0);
    recv(sockDescriptor,(char*)&opCode,sizeof(int),0);
    char buf[40];
    recv(sockDescriptor,buf,sizeof(buf),0);
    QString userName(buf);
    dataBase & db = dataBase::getInstance();
    userId= db.getUserId(userName);
    if (userId < 0){
        cout<<"auth fail\n";
        cout << " ";
        return false;
    }
    else{
        cout<<"auth success\n";
        return true;
    }
}
