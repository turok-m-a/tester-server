#include <iostream>
#include <winsock2.h>
using namespace std;

int main()
{
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
    send(s,(char*)&userType,4,0);
    send(s,userName,40,0);


    return 0;
}
