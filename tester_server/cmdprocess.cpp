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
    default:
        break;
    }
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
}
