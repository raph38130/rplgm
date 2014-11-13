/*
 * client UDP
 * need xinited with ubuntu linux
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

//#define PORT	33333 	//serveur_udp
#define PORT	13	//daytime
//#define PORT	19 	//chargen
//#define PORT	7 	//echo

#define LEN	80

int main()
{
int s,n,len,i=0;
struct sockaddr_in adr;
char buffer[LEN];

s=socket(AF_INET,SOCK_DGRAM,0);
if (s==-1) printf("erreur socket\n");

adr.sin_family		= AF_INET;
adr.sin_port		= htons(PORT);
adr.sin_addr.s_addr	= inet_addr("172.16.116.5");

len=sizeof(adr);
sendto(s,buffer,strlen(buffer),0,(struct sockaddr *) (&adr),len);
	
len=sizeof(adr);
n = recvfrom(s,buffer,LEN,0,(struct sockaddr *) (&adr),&len);
if (n==-1) printf("erreur recvfrom\n");
else buffer[n]=0;
printf("lu sur le port %d %d caractères : %s\n",PORT,n,buffer);
	
}



