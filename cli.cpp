#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#define DEFAULT_BUFLEN 512

using namespace std;

int main(int argc, char **argv) 
{
    WSADATA wsaData;

    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    char connectedClient[256];
    

	/*****************************************************/
	/*    Khoi tao Winsock 2.2
	/*****************************************************/
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
	cout<<"WSAStartup OK"<<endl;
	/*****************************************************/
	/*    Creating a Socket for the Server: ListenSocket */
	/*****************************************************/
    SOCKET sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sc == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
	cout<<"ConnectSocket = socket OK"<<endl;
	/*****************************************************/
	/*    */
	/*****************************************************/
	//Xac dinh dinh danh ung dung dich (destination)
	u_short port = 23;
	const char *IP="127.0.0.1";//"10.170.72.81"
	SOCKADDR_IN svrAddr;
	int svrAddrLen = sizeof(svrAddr);
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(port);
	svrAddr.sin_addr.s_addr = inet_addr(IP);
	
	if (connect(sc, (SOCKADDR*) &svrAddr, svrAddrLen) == SOCKET_ERROR) {
		printf ("\nCLIENT: Ket noi that bai\n");
		system("pause");
		WSACleanup();
		return 0;
	}else{
		printf("\nCLIENT: Ket noi thanh cong\n");
		printf("-----------------------------------------------------------------------\n");
	}
	//Gui nhan du lieu
	//Nhap ten
	int n;
	int a[1000];
	cout << "Nhap so n";
	cin>>n;
	for(int i = 0 ;i < n;i++){
		cout<<"Nhap so thu i: "<<endl;
		cin>>a[i];
	}	

	send(sc, (char*)&n, sizeof(int), 0);
	for (int i = 0; i < n; ++i) {
		send(sc, (char*)&a[i], sizeof(int), 0);
	}
	for (int i = 0; i < n; ++i) {
		recv(sc, (char*)&a[i], sizeof(int), 0);
	}
	for (int i = 0; i < n; ++i) {
		cout<<a[i]<<" ";
	}
    // cleanup
    closesocket(sc);
    WSACleanup();
	getchar();
    return 0;
}

