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

    int userType = 1;//студент
    int opCode = 1;//получить вопросы
    char userName[40];
    strcpy_s(userName,studLogin.toStdString().c_str());
    char * questionBuf;
    send(s,(char*)&userType,4,0);
    send(s,(char*)&opCode,4,0);
    send(s,userName,40,0);
    int questionsLen;
    recv(s,(char*)&questionsLen,sizeof(int),0);
    questionBuf = new char[questionsLen];
    int bytesReceived = recv(s,questionBuf,questionsLen,0);
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
    delete questionBuf;
    return questionsContainer;
}

int Network::sendQuestions(QVector<Question> questions)
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
    int opCode = 2;//отправить ответы
    char userName[40];
    strcpy_s(userName,studLogin.toStdString().c_str());
    char * questionBuf;
    send(s,(char*)&userType,4,0);
    send(s,(char*)&opCode,4,0);
    send(s,userName,40,0);
    Question question;
    foreach (question, questions) {
        int type = question.type;
        send(s,(char*)&type,4,0);
        if (type == 1){
            int number = question.getSelectedAnswers().size();
            number*=sizeof(int);
            send(s,(char*)&number,4,0);
            int answer;
            foreach (answer, question.getSelectedAnswers()) {
                send(s,(char*)&answer,4,0);
            }
        }
        if (type == 2){
            int textLen = strlen( question.getAnswerText().toStdString().c_str());
            send(s,(char*)&textLen,4,0);
            send(s,question.getAnswerText().toStdString().c_str(),textLen,0);
        }
        if (type == 3){
            int number = question.getAnswersSequence().size();
            number*=sizeof(int);
            send(s,(char*)&textLen,4,0);
            int answer;
            foreach (answer, question.getAnswersSequence()) {
                send(s,(char*)&answer,4,0);
            }
        }

    }
    closesocket(s);
}
