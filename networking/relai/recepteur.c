/*
 * client UDP
 * RP 17/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

#define PORT	2223 	//relai (coté reémission)
#define LEN	80

void main()
{
int s,n,len;
struct sockaddr_in adr;
char buffer[LEN];
struct hostent *h;

h=gethostbyname("localhost");
if (h==NULL) printf("erreur gethostbyname\n");

s=socket(AF_INET,SOCK_DGRAM,0);
if (s==-1) printf("erreur socket\n");

adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
bcopy(h->h_addr,&adr.sin_addr,h->h_length);

//un sendto pour l'init. de la socket
strcpy(buffer,"ini");
len=sizeof(adr);
sendto(s,buffer,strlen(buffer),0,(struct sockaddr *) (&adr),len);

while(1){
	len=sizeof(adr);
	n = recvfrom(s,buffer,LEN,0,(struct sockaddr *) (&adr),&len);
	if (n==-1) printf("erreur recvfrom\n");
	else buffer[n]=0;
	printf("lu sur le port %d %d caractères : %s\n",PORT,n,buffer);
	}
}


