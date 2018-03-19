#include "network.h"

Network::Network()
{
    WSADATA lp;
    WSAStartup(MAKEWORD( 2, 2 ),&lp);
}

void Network::setAddress(QString address)
{
    serverAddress = address;
}

void Network::setUser(QString _login, QString _password)
{
    login = _login;
    password = _password;
}

int Network::checkUser()
{
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN serverInfo;
    LPHOSTENT hostEnt;

    hostEnt = gethostbyname(serverAddress.toStdString().c_str());
    if (hostEnt == NULL){
        #ifdef WIN32
        WSACleanup();
        #endif
        int loginStatus = 3; //ошибка соединения
        return loginStatus;
    }
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
                int loginStatus = 3; //ошибка соединения
                return loginStatus;
            }
            int userType = 2;//препод/админ
            int opCode = NOP;
            char userName[40];
            strcpy_s(userName,login.toStdString().c_str());
            char _password[40];
            strcpy_s(_password,password.toStdString().c_str());
            send(s,(char*)&userType,4,0);
            send(s,(char*)&opCode,4,0);
            send(s,userName,40,0);
            send(s,_password,40,0);
       int loginStatus;
       recv(s,(char*)&loginStatus,sizeof(int),0);
       return loginStatus;
       closesocket(s);
}

QByteArray Network::sendQuery(int opCode, QByteArray query)
{

    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN serverInfo;
    LPHOSTENT hostEnt;

    hostEnt = gethostbyname(serverAddress.toStdString().c_str());
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
                return QByteArray();
            }
            int userType = 2;//препод/админ
            int _opCode = opCode;
            char userName[40];
            strcpy_s(userName,login.toStdString().c_str());
            char _password[40];
            strcpy_s(_password,password.toStdString().c_str());
            send(s,(char*)&userType,4,0);
            send(s,(char*)&_opCode,4,0);
            send(s,userName,40,0);
            send(s,_password,40,0);
       int loginStatus;
       recv(s,(char*)&loginStatus,sizeof(int),0);
       int replySize,querySize;
       querySize = query.size();
       send(s,(char*)&querySize,sizeof(int),0);
       send(s,query.data(),query.size(),0);
       recv(s,(char*)&replySize,sizeof(int),0);
       QByteArray reply;
       reply.resize(replySize);
       if (replySize) {
            recv(s,reply.data(),replySize,0);
       }
       closesocket(s);
       return reply;
}

QVector<Question> Network::getQuestionsForStudent(QString studLogin, int &loginStatus)
{
    QVector<Question> questionsContainer;
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN serverInfo;
    LPHOSTENT hostEnt;

    hostEnt = gethostbyname("127.0.0.1");
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
                loginStatus = 3; //ошибка соединения
                return questionsContainer;//нет вопросов
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
    if (bytesReceived <= sizeof(int)){
        loginStatus = 2;//нет открытого для сдачи экзамена
        return questionsContainer;
    }
    if (bytesReceived > sizeof(int)){
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

float Network::sendQuestions(QVector<Question> questions)
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
                return -1;
            }

    int userType = 1;
    int opCode = 2;//отправить ответы
    char userName[40];
    strcpy_s(userName,studLogin.toStdString().c_str());
    char * questionBuf;
    send(s,(char*)&userType,4,0);
    send(s,(char*)&opCode,4,0);
    send(s,userName,40,0);
    int questionNum = questions.size();
    send(s,(char*)&questionNum,4,0);
    Question question;
    foreach (question, questions) {
        int type = question.type;
        send(s,(char*)&type,4,0);
        int id = question.getId();
        send(s,(char*)&id,4,0);
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
        if (type == 3 || type == 4){
            int number = question.getAnswersSequence().size();
            number*=sizeof(int);
            send(s,(char*)&number,4,0);
            int answer;
            foreach (answer, question.getAnswersSequence()) {
                send(s,(char*)&answer,4,0);
            }
        }

    }
    float mark;
    recv(s,(char*)&mark,sizeof(float),0);
    closesocket(s);
    return mark;
}
