/*
 * serveur TCP
 *
 * RP 17/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT	4444
#define LEN	80

void main()
{
int s,s2,n,len,f;
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

//main loop
while(1) {
s2=accept(s,(struct sockaddr *) (&from),&len);
if (s2==-1) printf("erreur accept\n");

printf("connexion de %d.%d.%d.%d\n",
			 from.sin_addr.s_addr        & 0xFF,
			(from.sin_addr.s_addr >>  8) & 0xFF,
			(from.sin_addr.s_addr >> 16) & 0xFF,
			 from.sin_addr.s_addr >> 24
			);
//réception du nom de fichier
n=recv(s2,buffer,LEN,0);
buffer[n]=0;
strcat(buffer,".bak");
printf("fichier enregistré sous le nom %s\n",buffer);
f=open(buffer,O_CREAT|O_RDWR,S_IRWXU);

while(1){
	n=recv(s2,buffer,LEN,0);
	if (n==0) {
		printf("fin de réception\n");
		break;
		}
	if (n==-1) printf("erreur recv\n");
	else buffer[n]=0;
	write(f,buffer,n);
	}
n=shutdown(s2,2);
if (n==-1) printf("erreur shutdown\n");
} //end main loop
}


