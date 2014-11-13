// DemoNet1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "winsock2.h"



int _tmain(int argc, _TCHAR* argv[])
{
  WSADATA wsaData;
  SOCKET SendSocket;
  sockaddr_in RecvAddr,from;
  int Port = 13;
  char SendBuf[1024]="hello from rp/lgm...\n";
  int BufLen = 1024,l=4,res;

  //---------------------------------------------
  // Initialize Winsock
  WSAStartup(MAKEWORD(2,2), &wsaData);

  //---------------------------------------------
  // Create a socket for sending data
  SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  //---------------------------------------------
  // Set up the RecvAddr structure with the IP address of
  // the receiver (in this example case "123.456.789.1")
  // and the specified port number.
  RecvAddr.sin_family = AF_INET;
  RecvAddr.sin_port = htons(Port);
  RecvAddr.sin_addr.s_addr = inet_addr("192.168.0.2");

  //---------------------------------------------
  // Send a datagram to the receiver
  printf("sending : %s\n", SendBuf);
  sendto(SendSocket, 
    SendBuf, 
	strlen(SendBuf), 
    0, 
    (SOCKADDR *) &RecvAddr, 
    sizeof(RecvAddr));

  res=recvfrom(SendSocket,
	  SendBuf,
	  100,0,
	  (sockaddr*)&from,
	  &l);

  //---------------------------------------------
  // When the application is finished sending, close the socket.
  printf("%d\n %s",res,SendBuf);
  closesocket(SendSocket);

  //---------------------------------------------
  // Clean up and quit.
  printf("Exiting.\n");
  WSACleanup();
  getchar();
	return 0;
}

