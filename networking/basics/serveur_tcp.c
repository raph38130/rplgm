/*
 * serveur TCP
 *
 * version avec un seul client servi
 *
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>


#define PORT	2222

#define LEN	80

void main()
{
int s,s2,n,len,ipad,i=0;
struct sockaddr_in adr,from;
char buffer[LEN];
struct hostent *h;

s=socket(AF_INET,SOCK_STREAM,0);
if (s==-1) printf("erreur socket\n");
adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
adr.sin_addr.s_addr=INADDR_ANY;

n=bind(s,(struct sockaddr *)(&adr),sizeof(struct sockaddr));
if (n==-1) printf("erreur bind\n");

n=listen(s,1);
if (n==-1) printf("erreur listen\n");

s2=accept(s,(struct sockaddr *) (&from),&len);
if (s2==-1) printf("erreur accept\n");

ipad = ntohl(from.sin_addr.s_addr);
printf("connexion de %d.%d.%d.%d\n",
			 ipad >> 24,
			(ipad >> 16) & 0xFF,
			(ipad >>  8) & 0xFF,
			 ipad 	     & 0xFF
			);

while(1){
	//réception sur S2 obtenue par accept!!!
	n=recv(s2,buffer,LEN,0);
	if (n==-1) printf("erreur recv\n");
	else buffer[n]=0;
	printf("lu par le serveur sur le port %d %d caractères : %s\n",PORT,n,buffer);
	getchar();

	//émission sur S2 obtenue par accept!!!
	sprintf(buffer,"SERVEUR %d FIN",i++);
	n=send(s2,buffer,strlen(buffer),0);
	if (n==-1) printf("erreur send\n");
	}
}



