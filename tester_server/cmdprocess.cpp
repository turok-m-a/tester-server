#include "cmdprocess.h"

CmdProcess::CmdProcess()
{
}

CmdProcess::CmdProcess(int opCode, SOCKET _sock)
{
    sock = _sock;
    switch (opCode) {
    case GET_STUDENT_LIST:
        sendStudList();
        break;
    case ADD_STUDENT:
        addStudToList();
        break;
    case REMOVE_STUDENT:
        removeStudentFromList();
        break;
    case ADD_SUBJECT:
        addSubject();
        break;
    case GET_SUBJECT_LIST:
        findSubject();
        break;
    case REMOVE_SUBJECT:
        removeSubject();
        break;
    case GET_QUESTION_LIST:
        getQuestionList();
        break;
    case REMOVE_QUESTION:
        removeQuestion();
        break;
    case EDIT_SUBJECT_LIST_FOR_QUESTION:
        editQuestionSubjects();
        break;
    case ADD_QUESTION:
        addQuestion();
        break;
    case ADD_EXAM:
        addExam();
        break;
    case DELETE_EXAM:
        deleteExam();
        break;
    case SET_EXAM_TIME:
        setExamTime();
        break;
    case GET_EXAM_LIST:
        getExamList();
        break;
    case GET_PASS_STATE:
        getStudPassStatus();
        break;
    case START_EXAM_FOR_STUDENT:
        startExamForStudent();
        break;
    default:
        break;
    }
}
void CmdProcess::removeQuestion()
{
    int byteArrayLen;
    QByteArray byteArray,reply;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    int questionId;
    stream >> questionId;
    dataBase & db = dataBase::getInstance();
    db.removeQuestion(questionId);
    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}

void CmdProcess::editQuestionSubjects()
{
    int byteArrayLen;
    QByteArray byteArray,reply;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);

    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    int questionId,editOperationType,subjId;
    stream >> questionId >> editOperationType >> subjId;
    dataBase & db = dataBase::getInstance();
    db.editQuestionSubjects(questionId,editOperationType,subjId);
    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}

void CmdProcess::addQuestion()
{
    int byteArrayLen;
    QByteArray byteArray,reply;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);

    int questionType,subjectId,difficulty;
    QString questionText,answerText;
    QByteArray advData;
    stream >> questionType >> subjectId >> questionText >> answerText;
    if (questionType == SEQUENCE_QUESTION_TYPE || questionType == MATCH_QUESTION_TYPE){
        stream >> advData >> difficulty;
    }
    dataBase & db = dataBase::getInstance();
    db.addQuestion(questionType,subjectId,questionText,answerText,advData,difficulty);
    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}

void CmdProcess::addExam()
{
    int byteArrayLen;
    QByteArray byteArray,reply;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    int subjectId;
    QDate date;
    QByteArray questionList;
    stream >> subjectId >> date >> questionList;
    dataBase & db = dataBase::getInstance();
    db.addExam(subjectId,date,questionList);
    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}

void CmdProcess::setExamTime()
{
    QByteArray request = recvRequest();
    QDataStream stream(&request, QIODevice::ReadWrite);
    int time,id;
    stream >> time >> id;
    dataBase & db = dataBase::getInstance();
    bool alreadySet;
    db.setExamTime(time,id,alreadySet);
    QVector<QVector<QString>> status;
    if(alreadySet){
        dataBase & db = dataBase::getInstance();
        status = db.getExamStudList(id);
        QByteArray reply;
        QDataStream replyStream(&reply, QIODevice::ReadWrite);
        replyStream << status;
        sendReply(reply);
        return;
    }
    sendReply();
}

void CmdProcess::deleteExam()
{
    QByteArray request = recvRequest();
    QDataStream stream(&request, QIODevice::ReadWrite);
    int id;
    stream >> id;
    dataBase & db = dataBase::getInstance();
    db.deleteExam(id);
    sendReply();
}

void CmdProcess::startExamForStudent()
{
    QByteArray request = recvRequest();
    QDataStream stream(&request, QIODevice::ReadWrite);
    int examId,studNum,studId;
    stream >> studNum >> examId;
    QVector<int> studIds,passIds;
    for (int i=0;i<studNum;i++){
        stream >> studId;
        studIds.push_back(studId);
    }
    dataBase & db = dataBase::getInstance();
    db.startExamForStudent(studIds,examId,passIds);
    QByteArray reply;
    QDataStream replyStream(&reply, QIODevice::ReadWrite);
    replyStream << passIds;
    sendReply(reply);
}

void CmdProcess::getExamList()
{
    QByteArray request = recvRequest();
    QByteArray reply;
    QDataStream stream(&request, QIODevice::ReadWrite);
    QDataStream replyStream(&reply, QIODevice::ReadWrite);
    int subjId;
    stream >> subjId;
    QVector<QVector<QString>> exams;
    dataBase & db = dataBase::getInstance();
    exams = db.getExamList(subjId);
    replyStream << exams;
    sendReply(reply);
}

QByteArray CmdProcess::recvRequest()
{
    int byteArrayLen;
    QByteArray byteArray;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    return byteArray;
}

void CmdProcess::sendReply(QByteArray reply)
{
    const int replySize = reply.size();//длина в байтах
    send(sock,(char*)&replySize,sizeof(int),0);
    if (replySize) {
    send(sock,reply.data(),replySize,0);
    }
}

void CmdProcess::getStudPassStatus()
{
    QByteArray request = recvRequest();
    QDataStream stream(&request, QIODevice::ReadWrite);
    QVector<int> passId;
    QVector<float> status;
    stream >> passId;
    dataBase & db = dataBase::getInstance();
    status = db.getStudentPassStatus(passId);
    QByteArray reply;
    QDataStream replyStream(&reply, QIODevice::ReadWrite);
    replyStream << status;
    sendReply(reply);
}
void CmdProcess::getQuestionList()
{
    int byteArrayLen;
    QByteArray byteArray,reply;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    int subjId;
    stream >> subjId;
    dataBase & db = dataBase::getInstance();
    QVector<QVector <QString>> questions = db.getQuestions(subjId);
    QDataStream replyStream(&reply, QIODevice::ReadWrite);
    replyStream << questions.size(); //кол-во вопросов
    replyStream << questions;
    const int replySize = reply.size();//длина в байтах
    send(sock,(char*)&replySize,sizeof(int),0);
    send(sock,reply.data(),replySize,0);
}


void CmdProcess::sendStudList()
{
    int filterParamNum,byteArrayLen;
   // recv(sock,(char*)&filterParamNum,sizeof(int),0);//число параметров
    QVector<int> filterParams;
    QVector<QString> filterValues;


    QByteArray byteArray;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream >> filterParamNum; //кол-во пар-ров
    for (int i=0;i<filterParamNum;i++){
        int param;
        QString value;
          stream >> param;
          stream >> value;
       filterParams.push_back(param);
       filterValues.push_back(value);
    }
    dataBase & db = dataBase::getInstance();
    QVector<QVector <QString>> students = db.findStudents(filterParams,filterValues);

    QByteArray reply;
    QDataStream stream2(&reply, QIODevice::WriteOnly);
    stream2 << students.size();
    for (int i=0;i<students.size();i++){
        for (int j=0;j<6;j++){
            stream2 << students[i][j];
        }
    }
    int size = reply.size();
    send(sock,(char*)&size,sizeof(int),0);
    send(sock,reply.data(),size,0);

}

void CmdProcess::addStudToList()
{
    int byteArrayLen;
    QVector<int> params;
    QVector<QString> values;

    QByteArray byteArray;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);

    for (int i=0;i<5;i++){
        int param;
        QString value;
          stream >> param;
          stream >> value;
       params.push_back(param);
       values.push_back(value);
    }
    dataBase & db = dataBase::getInstance();
    db.addStudent(values);
    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}

void CmdProcess::removeStudentFromList()
{
    int byteArrayLen;
    QByteArray byteArray;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);
    int id;
    stream >> id;
    dataBase & db = dataBase::getInstance();
    db.removeStudent(id);

    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}

void CmdProcess::addSubject()
{
    int byteArrayLen;
    QVector<int> params;
    QVector<QString> values;

    QByteArray byteArray;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    QString subjName;
    stream >> subjName;
    dataBase & db = dataBase::getInstance();
    db.addSubject(subjName);
    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}

void CmdProcess::findSubject()
{
    int byteArrayLen;

    QByteArray byteArray,reply;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    QDataStream replyStream(&reply, QIODevice::WriteOnly);
    QString subjName;
    stream >> subjName;
    dataBase & db = dataBase::getInstance();
    QVector<QVector<QString>> subjList = db.findSubject(subjName);
    replyStream << subjList.size();

    for (int i=0;i<subjList.size();i++){
        for (int j=0;j<2;j++){
            replyStream << subjList[i][j];
        }
    }
    const int replySize = reply.size();
    send(sock,(char*)&replySize,sizeof(int),0);
    send(sock,reply.data(),replySize,0);
}

void CmdProcess::removeSubject()
{
    int byteArrayLen;


    QByteArray byteArray;
    recv(sock,(char*)&byteArrayLen,sizeof(int),0);//длина запроса
    byteArray.resize(byteArrayLen);
    recv(sock,byteArray.data(),byteArrayLen,0);
    QDataStream stream(&byteArray, QIODevice::ReadWrite);
    int studId;
    stream >> studId;
    dataBase & db = dataBase::getInstance();
    db.delSubject(studId);
    const int replySize = 0;
    send(sock,(char*)&replySize,sizeof(int),0);
}


