// Daytimed.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;


int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"daytimed running");
try
{
	//IPEndPoint object will allow us to read datagrams sent from any source.
   IPEndPoint^ RemoteIpEndPoint = gcnew IPEndPoint( IPAddress::Any,0 );
   UdpClient ^ udpClient=gcnew UdpClient(1313);
   // Block until a message returns on this socket from a remote host.
   array<Byte>^receiveBytes = udpClient->Receive( RemoteIpEndPoint );
   String^ returnData = Encoding::ASCII->GetString( receiveBytes );

   // Use the IPEndPoint object to determine which of these two hosts responded.
   Console::WriteLine( String::Concat( "This is the message you received ", returnData->ToString() ) );
   Console::WriteLine( String::Concat( "This message was sent from ", RemoteIpEndPoint->Address->ToString(), " on their port number ", RemoteIpEndPoint->Port.ToString() ) );
   
   array<Byte>^sendBytes = Encoding::ASCII->GetBytes(DateTime::Now.ToLocalTime().ToString());
   udpClient->Send(sendBytes, sendBytes->Length ,RemoteIpEndPoint);
   udpClient->Close();
}
catch ( Exception^ e ) 
{
   Console::WriteLine( e->ToString() );
}
Console::Read();
    return 0;
}
