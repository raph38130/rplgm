/* LightClient.c
 *
 * projet ESRF/LGM
 *
 * un client SOAP léger pour TANGO
 *
 * RP
 * 	- cvs test
 *	- second diff
 *	- third edition
 *	- 4th and 5th to tes $id   $Id: LightClient.c,v 1.1 2001/03/15 12:00:21 raph Exp $
 *
 * RP Mon Feb 12 15:01:45 CET 2001
 *	- version initiale
 *	- remarque : ascii(\042) = "
 *
 *
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

#define LEN		1024

/*soap request*/
#define PORT		8080
//#define SERVERNAME	"mercure"
#define SERVERNAME	"172.16.104.2"
#define HEADER		"\
POST /soap/servlet/rpcrouter HTTP/1.0
Content-Type: text/xml; charset=utf-8
Content-Length: %d
SOAPAction: \042\042\n\n"

#define SOAPREQ		"
<s:Envelope
	xmlns:s=\042http://schemas.xmlsoap.org/soap/envelope/\042
	xmlns:xsi=\042http://www.w3.org/1999/XMLSchema-instance\042
	xmlns:xsd=\042http://www.w3.org/1999/XMLSchema\042
	>
<s:Body>
<command_list_query xmlns=\042urn:Soap2Tango\042>
<devname 	xsi:type=\042xsd:string\042
		s:encodingStyle=\042http://schemas.xmlsoap.org/soap/encoding/\042
		>id1/DSClock/1</devname>
</command_list_query>
</s:Body>
</s:Envelope>"

int main()
{
int s,n,len;
struct sockaddr_in adr;
char req[LEN],ans[LEN];
struct hostent *h;

printf("$Id: LightClient.c,v 1.1 2001/03/15 12:00:21 raph Exp $");
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
sprintf(req,HEADER,strlen(SOAPREQ));
strcat(req,SOAPREQ);
//printf("%s",req);
n=send(s,req,strlen(req),0);
if (n==-1) printf("erreur send\n");
//réception réponse serveur WEB
do	{
	n=recv(s,ans,LEN,0);
	if (n==-1) printf("erreur recv\n");
	else ans[n]=0;
	printf("%s",ans);
	}
while(n>0);	//TODO

return 0;
}
