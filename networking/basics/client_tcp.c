/*
 * client TCP
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

//#define PORT	13 	//daytime
#define PORT	2222 	//serveur_tcp
//#define PORT	7 	//echo

#define LEN	80

void main()
{
int s,n,len,i=0;
struct sockaddr_in adr;
char buffer[LEN];
struct hostent *h;

h=gethostbyname("localhost");
if (h==NULL) printf("erreur gethostbyname\n");

s=socket(AF_INET,SOCK_STREAM,0);
if (s==-1) printf("erreur socket\n");

adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
bcopy(h->h_addr_list[0],&adr.sin_addr,h->h_length);
//adr.sin_addr.s_addr=inet_addr("172.16.0.3");
n=connect(s,(struct sockaddr *) (&adr),sizeof(adr));
if (n==-1) printf("erreur connect\n");

while(1){
	//émission
	sprintf(buffer,"msg%d",i++);
	n=send(s,buffer,strlen(buffer),0);
	if (n==-1) printf("erreur send\n");
	//réception
	n=recv(s,buffer,LEN,0);
	if (n==-1) printf("erreur recv\n");
	else buffer[n]=0;

	printf("lu sur le port %d %d caractères : %s\n",PORT,n,buffer);
	getchar();
	}
}



