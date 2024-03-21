#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <fstream> 

#define DEFAULT_BUFLEN 512
using namespace std;



void readStudent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Kiểm tra xem dòng có đúng định dạng không
        size_t pos1 = line.find("#");
        size_t pos2 = line.find("#", pos1 + 1);
        size_t pos3 = line.find("#", pos2 + 1);
        if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos) {
            std::cerr << "Invalid format in line: " << line << std::endl;
            break;
        }

        // Xử lý thông tin từ dòng
        std::string MASV = line.substr(0, pos1);
        std::string tenSV = line.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string soDienThoai = line.substr(pos2 + 1, pos3 - pos2 - 1);
        std::string emailSV = line.substr(pos3 + 1);

        // Xử lý thông tin đọc được ở đây
        std::cout << "MASV: " << MASV << std::endl;
        std::cout << "Ten sinh vien: " << tenSV << std::endl;
        std::cout << "So dien thoai: " << soDienThoai << std::endl;
        std::cout << "Email cua SV: " << emailSV << std::endl;
        std::cout << std::endl;
    }

    file.close();
}

int main() {
    int iResult=0;
    
/*****************************************************/
/*    Khoi tao Winsock 2.2
/*****************************************************/
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    	cout << "\nWINSOCK: Khoi tao that bai\n";
    	WSACleanup();
    	return 0;
	}	else	{
		cout << "\nWINSOCK: Khoi tao thanh cong\n";
	}
/*****************************************************/
/*    Creating a Socket for the Server: ListenSocket */
/*****************************************************/
	SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
	    printf("Error at socket(): %ld\n", WSAGetLastError());
	    WSACleanup();
	    return 1;
	}
/*****************************************************/
/*    Binding a Socket*/
/*****************************************************/
	//Xac dinh dinh danh ung dung 
    u_short port = 23;
    const char *IP = "127.0.0.1";
    SOCKADDR_IN svrAddr;
	int svrAddrLen = sizeof(svrAddr);
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(port);
	svrAddr.sin_addr.s_addr = inet_addr(IP);
    // Setup the TCP listening socket
    iResult = bind( ListenSocket, (SOCKADDR*) &svrAddr, svrAddrLen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    cout<<"ListenSocket: "<<ListenSocket<<endl;
    
/*****************************************************/
/*    Listening on a Socket*/
/*****************************************************/
	
	if ( listen(ListenSocket, 5 ) == SOCKET_ERROR ) {
	    printf( "Listen failed with error: %ld\n", WSAGetLastError() );
	    closesocket(ListenSocket);
	    WSACleanup();
	    return 1;
	}

    cout<<"Listening ... "<<endl;
	readStudent("input.txt");
	do{
		/************************************************/
		/*    Accepting a Connection*/
		/************************************************/

		// Accept a client socket
		SOCKADDR_IN cliAddr;
		int cliAddrLen = sizeof(cliAddr);
		SOCKET sa = accept(ListenSocket, (SOCKADDR*) &cliAddr, &cliAddrLen);
		cout<<"Nhan du lieu tu IP:"<< inet_ntoa(cliAddr.sin_addr) << " port: "<< ntohs(cliAddr.sin_port)<<endl;

		if (sa == INVALID_SOCKET) {
				printf("accept failed: %d\n", WSAGetLastError());
				closesocket(sa );
				WSACleanup();
				return 1;
		}
		/************************************************/
		/*    Receiving and Sending Data on the Server*/
		/************************************************/
		int n;
		recv(sa, (char*)&n, sizeof(int), 0);

		// Nhận từng phần tử của mảng từ client
		int a[1000];
		for (int i = 0; i < n; ++i) {
			recv(sa, (char*)&a[i], sizeof(int), 0);
		}
		sort(a,a+n);

		for (int i = 0; i < n; ++i) {
			send(sa, (char*)&a[i], sizeof(int), 0);
		}
		
		iResult = shutdown(sa, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(sa);
			WSACleanup();
			return 1;
		}
		// cleanup
		closesocket(sa);
	}while(1);
		
	closesocket(ListenSocket);
	WSACleanup();
	getchar();
	return 0;
}//main
