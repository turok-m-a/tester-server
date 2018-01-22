#include "connection.h"

connection::connection()
{
    WSADATA lp;
    WSAStartup(MAKEWORD( 2, 2 ),&lp);
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN sin;
            sin.sin_family = PF_INET;
            sin.sin_port = htons(10001);
            sin.sin_addr.s_addr = INADDR_ANY;
        int retVal = bind(s, (LPSOCKADDR)&sin, sizeof(sin));
        if(retVal == SOCKET_ERROR)
                {
                    cout<<"Unable to bind\n";
                    #ifdef WIN32
                    cout << GetLastError();
                    WSACleanup();
                    #endif
                    return;
                }
        //Пытаемся начать слушать сокет
           retVal = listen(s, SOMAXCONN);
           if(retVal == SOCKET_ERROR)
           {
               cout<<"Unable to listen\n";
               #ifdef WIN32
               WSACleanup();
               #endif
               return;
           }
           while (true) {  //далее данный поток лишь принимает соединения
               SOCKET client = accept(s,NULL,NULL); //to do: print client addr to console
               connectionThread * thread = new connectionThread(client,NULL);
               QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); //для удаления по завершению потока
               thread->start();
          }
}




