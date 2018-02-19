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
    default:
        break;
    }
}

void CmdProcess::sendStudList()
{
    int filterParamNum,filterParam,paramLen;
    recv(sock,(char*)&filterParamNum,sizeof(int),0);//число параметров
    QVector<int> filterParams;
    QVector<QString> filterValues;
    for (int i=0;i<filterParamNum;i++){
        recv(sock,(char*)&filterParam,sizeof(int),0);
        filterParams.push_back(filterParam);
        recv(sock,(char*)&paramLen,sizeof(int),0);
        char * buf = new char[paramLen];
        recv(sock,buf,paramLen,0);
        filterValues.push_back(QString(buf));
        delete buf;
    }
    dataBase & db = dataBase::getInstance();
    QVector<QVector <QString>> students = db.findStudents(filterParams,filterValues);

    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);


    for (int i=0;i<students.size();i++){
        for (int j=0;j<6;j++){
            stream << students[i][j];
        }
    }
    int size = byteArray.size();
    send(sock,(char*)&size,sizeof(int),0);
    send(sock,byteArray.data(),size,0);

}
