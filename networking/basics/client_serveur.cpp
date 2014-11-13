/*
 * client UDP
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

#define PORT	2222 	//serveur_udp
//#define PORT	13 	//daytime
//#define PORT	19 	//chargen
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

s=socket(AF_INET,SOCK_DGRAM,0);
if (s==-1) printf("erreur socket\n");

adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
bcopy(h->h_addr,&adr.sin_addr,h->h_length);

while(1){
	//emission (necessaire pour réveiller le daemon
	//meme s'il n'utilise pas la donnée émise)
	sprintf(buffer,"hello%d",i++);
	len=sizeof(adr);
	sendto(s,buffer,strlen(buffer),0,(struct sockaddr *) (&adr),len);
	
	strcpy(buffer,"ca ne marche pas");

	//réception
	len=sizeof(adr);
	n = recvfrom(s,buffer,LEN,0,(struct sockaddr *) (&adr),&len);
	if (n==-1) printf("erreur recvfrom\n");
	else buffer[n]=0;
	
	printf("lu sur le port %d %d caractères : %s\n",PORT,n,buffer);

	getchar();
	}
}



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



