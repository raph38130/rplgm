/*
 * squelette de serveur UDP d�marr� par inetd
 *
 * serverport  dgram udp  wait eleve   udpserver
 *
 * RP 17/10/00
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <time.h>

#define LEN     80

main()
{
int s,n,len,i=0;
struct sockaddr_in adr;
char buffer[LEN];
struct hostent *h;
time_t t;

s=0;
//r�ception
n=recvfrom(s,buffer,LEN,0,(struct sockaddr *)(&adr),&len);
//�mission
time(&t);
sprintf(buffer,"la date sur le serveur est %s",ctime(&t));
n=sendto(s,buffer,strlen(buffer),0,(struct sockaddr *)(&adr),sizeof(struct sockaddr));
}

