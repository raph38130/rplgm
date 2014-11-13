/*
 * un client WEB simplissime
 *
 * RP 24/10/00	version initiale
 *    27/10/00	request by proxy server
 *
 * TODO : condition de fin à améliorer
 *
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>


#define PORT		80
#define SERVERNAME	"mercure"
#define REQUEST		"GET /index.html\n\n"

/* pour passer à travers le proxy */
/*
#define PORT		3128
#define SERVERNAME	"192.168.1.1"
#define REQUEST		"GET http://www.ibm.com/index.html HTTP/1.1\n\n"
*/
 
#define LEN		256

int main()
{
int s,n,len;
struct sockaddr_in adr;
char req[LEN],ans[LEN];
struct hostent *h;

h=gethostbyname(SERVERNAME);
if (h==NULL) printf("erreur gethostbyname\n");

s=socket(AF_INET,SOCK_STREAM,0);
if (s==-1) printf("erreur socket\n");

adr.sin_family=AF_INET;
adr.sin_port=htons(PORT);
bcopy(h->h_addr_list[0],&adr.sin_addr,h->h_length);
n=connect(s,(struct sockaddr *) (&adr),sizeof(adr));
if (n==-1) printf("erreur connect\n");

//interrogation du serveur WEB
strcpy(req,REQUEST);
n=send(s,req,strlen(req),0);
if (n==-1) printf("erreur send\n");

//réception réponse serveur WEB
do	{
	n=recv(s,ans,LEN,0);
	if (n==-1) printf("erreur recv\n");
	else ans[n]=0;
	printf("%s",ans);
	}
while(1);	//condition de fin à améliorer

return 0;
}
