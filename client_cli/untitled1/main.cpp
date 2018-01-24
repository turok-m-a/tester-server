#include <iostream>
#include <winsock2.h>
using namespace std;
#include <QString>
#include <QTextStream>
#include <QTextCodec>
#include <question.h>
int main()
{
    //system("chcp 65001 > NULL");//перекл. консоль в utf8
    QTextStream outStream(stdout,QIODevice::WriteOnly);
    outStream.setCodec(QTextCodec::codecForName("cp866"));
    WSADATA lp;
    WSAStartup(MAKEWORD( 2, 2 ),&lp);
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
                return 0;
            }

    int userType = 1;
    char userName[40] = {"4501111"};
    char questionBuf[64000];
    send(s,(char*)&userType,4,0);
    send(s,userName,40,0);
    recv(s,questionBuf,64000,0);
    char * questions = questionBuf;
     int q_number = *((int*)questions);
    questions+=4;
    QVector<Question> questionsContainer;
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
        questions+=adv_len;
    }
    QString del = questionsContainer[0].getQuestionText();

    return 0;
}
