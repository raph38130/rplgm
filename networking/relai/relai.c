/*
 * serveur UDP relayant deux machines
 *
 * RP 17/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>


#define PORT	2222	//(coté emetteur)
#define PORT2	2223	//(coté recepteur)
#define LEN	80

void main()
{
int s,t,n,len;
struct sockaddr_in adr,to;
char buffer[LEN];

s=socket(AF_INET,SOCK_DGRAM,0);
if (s==-1) printf("erreur socket\n");
adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
adr.sin_addr.s_addr=INADDR_ANY;

t=socket(AF_INET,SOCK_DGRAM,0);
if (t==-1) printf("erreur socket\n");
to.sin_family=AF_INET;
to.sin_port=htons(PORT2);
to.sin_addr.s_addr=INADDR_ANY;

n=bind(s,(struct sockaddr *)(&adr),sizeof(struct sockaddr));
if (n==-1) printf("erreur bind s\n");
n=bind(t,(struct sockaddr *)(&to),sizeof(struct sockaddr));
if (n==-1) printf("erreur bind t\n");

//pour l'init. de la socket recepteur, un recvfrom
n=recvfrom(t,buffer,LEN,0,(struct sockaddr *)(&to),&len);
if (n==-1) printf("erreur recvfrom t\n");

while(1){
	//réception
	n=recvfrom(s,buffer,LEN,0,(struct sockaddr *)(&adr),&len);
	if (n==-1) printf("erreur recv\n");
	else buffer[n]=0;
	printf("connexion de %d.%d.%d.%d par le port %d\n",
			 adr.sin_addr.s_addr >> 24,
			(adr.sin_addr.s_addr >> 16) & 0xFF,
			(adr.sin_addr.s_addr >>  8) & 0xFF,
			 adr.sin_addr.s_addr& 0xFF,
			 adr.sin_port
			);
	printf("recu sur le port %d %d caractères : %s\n",PORT,n,buffer);
	//reémission
	printf("reémission vers %d.%d.%d.%d port %d\n",
			 to.sin_addr.s_addr >> 24,
			(to.sin_addr.s_addr >> 16) & 0xFF,
			(to.sin_addr.s_addr >>  8) & 0xFF,
			 to.sin_addr.s_addr& 0xFF,
			 to.sin_port
			);
	n=sendto(t,buffer,strlen(buffer),0,(struct sockaddr *)(&to),sizeof(struct sockaddr));
	if (n==-1) printf("erreur send\n");
	}
}


