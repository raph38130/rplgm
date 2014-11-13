/* 
 * RP pour le LGM
 *
 *	script CGI 
 *
 * 	passage des paramètres en POST ou GET
 *	retour des résultats en HTML
 *
 * 	RP Sun Jun  4 21:18:53 CEST 2000
 *		- version initiale
 *	RP Thu Oct 25 12:33:22 CEST 2001
 *		- complété avec version bash
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
char buffer[80];

//printf("method=%s\n",getenv("REQUEST_METHOD"));

if (!strcmp(getenv("REQUEST_METHOD"),"POST")) {
	//methode POST
	scanf("%s",buffer);
	printf("Content-type: text/html\n\n<HTML> Paramètres de la requête POST %s<BR></HTML>\n",buffer);
	}
else
if (!strcmp(getenv("REQUEST_METHOD"),"GET"))  {
	//methode GET
	printf("Content-type: text/html\n\n<HTML> Paramètres de la requête GET %s<BR></HTML>\n",getenv("QUERY_STRING"));
	}
else printf("Content-type: text/html\n\n<HTML> METHOD ERROR <BR></HTML>\n");
return 0;
}
