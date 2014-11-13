/*
 * client UDP
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

#define PORT	2222 	//relai (coté emetteur)
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

while(1){
	printf("transmettre : ");
	scanf("%s",buffer);
	len=sizeof(adr);
	sendto(s,buffer,strlen(buffer),0,(struct sockaddr *) (&adr),len);
	}
}


