/*
 * client TCP
 * RP 11/10/98
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>

#define PORT	4444
#define LEN	80

void main(int argc, char **argv)
{
int s,n,len,f;
struct sockaddr_in adr;
char buffer[LEN];
struct hostent *h;

if (argc!=3) {
	printf("usage : save file host\n");
	exit();
	}
	
f=open(argv[1],O_RDONLY);

h=gethostbyname(argv[2]);
if (h==NULL) printf("erreur gethostbyname\n");

s=socket(AF_INET,SOCK_STREAM,0);
if (s==-1) printf("erreur socket\n");

adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
bcopy(h->h_addr,&adr.sin_addr,h->h_length);

n=connect(s,(struct sockaddr *) (&adr),sizeof(adr));
if (n==-1) printf("erreur connect\n");

//transmission du nom de fichier
n=send(s,argv[1],strlen(argv[1]),0);
if (n==-1) printf("erreur send\n");

while(1){
	n=read(f,buffer,LEN);
	if (n==0) {
		printf("terminé.\n");
		break;
		}
	n=send(s,buffer,n,0);
	if (n==-1) printf("erreur send\n");
	}
n=shutdown(s,2);
if (n==-1) printf("erreur shutdown\n");
}


