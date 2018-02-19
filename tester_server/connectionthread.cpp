#include "connectionthread.h"

void connectionThread::processStudent()
{
    char buf[40];//username
    recv(sockDescriptor,buf,sizeof(buf),0);

    QString userName(buf);
    dataBase & db = dataBase::getInstance();
    userId= db.getUserId(userName);
    if (userId < 0){
        int code = 2;//не найден
        send(sockDescriptor,(char*)&code,4,0);
        closesocket(sockDescriptor);
        return;
    }


    int subject_id,status,select_type;
    status = db.getStudentCurrentExamState(userId,subject_id,select_type,question_list,exam_id);
    if (status != 11){
        int code = 2;//нет допуска
        send(sockDescriptor,(char*)&code,4,0);
        closesocket(sockDescriptor);
        return;
    }
    if (opCode == 2){
        processStudentAnswers();
        return;
    }
    if (select_type == 1) { //список вопросов заранее задан
        question_list = db.getQuestionsForExam(question_list); //получаем сами вопросы для передачи на клиент.
        int size = question_list.size();
        send(sockDescriptor,(char*)&size,sizeof(int),0);
        send(sockDescriptor,question_list.constData(),size,0);
    }
    //to-do: рандомные вопросы по предмету

}

void connectionThread::processTeacher()
{
    dataBase & db = dataBase::getInstance();
    char buf[40];//username
    recv(sockDescriptor,buf,sizeof(buf),0);
    QString userName(buf);
    //password
    recv(sockDescriptor,buf,sizeof(buf),0);
    QString password(buf);
    bool userExists = db.checkUser(userName,password);
    if (userExists) {
       int status = CONN_OK;
       send(sockDescriptor,(char*)&status,sizeof(int),0);
    } else {
        int status = CONN_NOT_FOUND;
        send(sockDescriptor,(char*)&status,sizeof(int),0);
        closesocket(sockDescriptor);
        return;
    }
    CmdProcess proc(opCode,sockDescriptor);
    closesocket(sockDescriptor);
}

void connectionThread::processStudentAnswers()
{
    dataBase & db = dataBase::getInstance();
    int type,id,questionNumber; //тип и id вопроса
    recv(sockDescriptor,(char*)&questionNumber,sizeof(int),0);
    int testMark = 0; //набранное кол-во баллов
    int maxMark = 0; //максимальное
    for (int i=0;i<questionNumber;i++){
    recv(sockDescriptor,(char*)&type,sizeof(int),0);
    recv(sockDescriptor,(char*)&id,sizeof(int),0);
    maxMark += db.getMaxMark(id);
    if (type == 1){
        int number;
        QVector<int> answers;
        recv(sockDescriptor,(char*)&number,4,0);
        number/=sizeof(int);
        int answer;
        for(int j=0;j<number;j++){
            recv(sockDescriptor,(char*)&answer,4,0);
            answers.push_back(answer);
        }
        testMark += db.checkAnswer(id,answers);
    }
    if (type == 2){
        int textLen;
        recv(sockDescriptor,(char*)&textLen,4,0);
        char * utf8string = new char[textLen+1];
        recv(sockDescriptor,utf8string,textLen,0);
        QString answer = QString::fromUtf8(utf8string,textLen);
        testMark += db.checkAnswer(id,answer);
    }
    if (type == 3 || type == 4){
        int number;
        QVector<int> answers;
        recv(sockDescriptor,(char*)&number,4,0);
        number/=sizeof(int);
        int answer;
        for(int j=0;j<number;j++){
            recv(sockDescriptor,(char*)&answer,4,0);
            answers.push_back(answer);
        }
        testMark += db.checkAnswer(id,answers);
    }
    }
    float decimalMark = round ((float)testMark / (float)maxMark * 100);
    decimalMark /= 10;
    std::cout <<"testmark " <<testMark ;
    send(sockDescriptor,(char*)&decimalMark,sizeof(float),0);
    closesocket(sockDescriptor);
}

connectionThread::connectionThread(SOCKET s, QObject *parent) :
    QThread(parent), sockDescriptor(s),userType (0) {}

void connectionThread::run()
{
    cout << "thread start\n";
    recv(sockDescriptor,(char*)&userType,sizeof(int),0);
    recv(sockDescriptor,(char*)&opCode,sizeof(int),0);
    switch (userType) {
    case 1:
        processStudent();
        break;
    case 2:
        processTeacher();
        break;
    default:
        break;
    }
}

bool connectionThread::authorization()
{

    return true;
}
