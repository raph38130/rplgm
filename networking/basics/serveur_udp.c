/*
 * serveur UDP
 *
 * si plusieurs clients, servis à tour de role indistinctement par le meme
 * process
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

//#define PORT	2222
#define PORT	67

#define LEN	80

void main()
{
int s,n,len,i=0;
struct sockaddr_in adr;
char buffer[LEN];
struct hostent *h;


s=socket(AF_INET,SOCK_DGRAM,0);
if (s==-1) printf("erreur socket\n");
adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
adr.sin_addr.s_addr=INADDR_ANY;

n=bind(s,(struct sockaddr *)(&adr),sizeof(struct sockaddr));
if (n==-1) printf("erreur bind\n");
printf("apres bind\n");
/* ne pas utiliser :
 *    listen(s,1);
 *    accept(s,(struct sockaddr *) (&from),&len);
 * uniquement avec SOCK_STREAM
 */

while(1){
	//réception
	n=recvfrom(s,buffer,LEN,0,(struct sockaddr *)(&adr),&len);
	if (n==-1) printf("erreur recv\n");
	else buffer[n]=0;
	printf("connexion de %d.%d.%d.%d\n",
			adr.sin_addr.s_addr         & 0xFF,
			(adr.sin_addr.s_addr >>  8) & 0xFF,
			(adr.sin_addr.s_addr >> 16) & 0xFF,
			 adr.sin_addr.s_addr >> 24
			);
	//printf("connexion de %s\n", inet_ntoa(adr.sin_addr.s_addr));
	printf("lu par le serveur sur le port %d %d caractères : %s\n",PORT,n,buffer);
	getchar();

	//émission
	sprintf(buffer,"SERVEUR %d FIN",i++);
	n=sendto(s,buffer,strlen(buffer),0,(struct sockaddr *)(&adr),sizeof(struct sockaddr));
	if (n==-1) printf("erreur send\n");
	}
}




