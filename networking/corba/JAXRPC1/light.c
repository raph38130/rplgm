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
#define SERVERNAME	"localhost"
#define HEADER		"\
POST /tango2-jaxrpc/tango2 HTTP/1.0
Content-Type: text/xml; charset=utf-8
Content-Length: %d
SOAPAction: \042\042\n\n"

#define SOAPREQ		"
<?xml version=\0421.0\042 encoding=\042UTF-8\042?>
<env:Envelope 

xmlns:env=\042http://schemas.xmlsoap.org/soap/envelope/\042 
xmlns:xsd=\042http://www.w3.org/2001/XMLSchema\042 
xmlns:xsi=\042http://www.w3.org/2001/XMLSchema-instance\042 
xmlns:enc=\042http://schemas.xmlsoap.org/soap/encoding/\042 
xmlns:ns0=\042http://lgm.ac-grenoble.fr/wsdl/Tango2\042 
xmlns:ns1=\042http://lgm.ac-grenoble.fr/types/Tango2\042 
env:encodingStyle=\042http://schemas.xmlsoap.org/soap/encoding/\042>

<env:Body><ns0:getserverlist/></env:Body></env:Envelope>"








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
printf("%s\n\n",req);
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
