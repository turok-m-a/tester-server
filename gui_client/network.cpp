#include "network.h"

Network::Network()
{
    WSADATA lp;
    WSAStartup(MAKEWORD( 2, 2 ),&lp);
}

QVector<Question> Network::getQuestionsForStudent(QString studLogin, int &loginStatus)
{
    QVector<Question> questionsContainer;
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN serverInfo;
    LPHOSTENT hostEnt;

    hostEnt = gethostbyname("localhost");
    serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
    serverInfo.sin_family = PF_INET;
     serverInfo.sin_port = htons(10001);
    int retVal=connect(s,(LPSOCKADDR)&serverInfo, sizeof(serverInfo));

            if(retVal==SOCKET_ERROR)
            {
                printf("Unable to connect\n");
                #ifdef WIN32
                WSACleanup();
                #endif
                return questionsContainer;
            }

    int userType = 1;
    char userName[40];
    strcpy_s(userName,studLogin.toStdString().c_str());
    char questionBuf[64000];
    send(s,(char*)&userType,4,0);
    send(s,userName,40,0);
    int bytesReceived = recv(s,questionBuf,64000,0);
    if (bytesReceived == sizeof(int)){
        loginStatus = 2;//нет открытого для сдачи экзамена
    }
    if (bytesReceived >= sizeof(int)){
        loginStatus = 1;//OK
    }
    closesocket(s);
    char * questions = questionBuf;
     int q_number = *((int*)questions);
    questions+=4;
    for (int i=0;i<q_number;i++){
        int q_id = *((int*)questions);
        questions+=4;
        int q_type = *((int*)questions);
        questions+=4;
        int q_len = *((int*)questions);
        questions+=4;
        QString question = QString::fromUtf8(questions,q_len);
        questionsContainer.push_back(Question(question,q_id,q_type));

        questions+=q_len;

        int adv_len = *((int*)questions);
        questions+=4;
        // adv data
        if (adv_len >0){
        QByteArray data(questions,adv_len);
        questionsContainer.last().addAdvancedData(data);
        questions+=adv_len;
        }
    }
    QString del = questionsContainer[0].getQuestionText();//debug
    return questionsContainer;
}
