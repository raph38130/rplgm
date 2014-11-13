/*
 * serveur TCP démarré automatiquemnt par inetd : socket 0
 *
 * 
 *
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>



#define LEN	80

void main()
{
int s2=0,	//fourni par inetd
	n,i=0;
char buffer[LEN];

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


/etc/inetd.conf
-=-=-=-=-=-=-=-
service            stream   tcp     nowait    eleve    /home/eleve/server 

/etc/services
-=-=-=-=-=-=- 
# Local services
service            4444/tcp                       #service : server

