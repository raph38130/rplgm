/*
 * squelette de serveur TCP d�marr� par inetd
 * un processus d�marr� pour chaque requ�te
 *
 * serverport  stream  tcp  nowait eleve   tcpserver
 *
 * RP 17/10/00
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>


#define LEN     80

main()
{
int s2=0,n,i=0;
char buffer[LEN];

for (i=0;i<5;i++) {
        //r�ception sur S2 obtenue par accept!!!
        n=recv(s2,buffer,LEN,0);

        //�mission sur S2 obtenue par accept!!!
        sprintf(buffer,"Lancement du serveur REUSSI %d\n",i);
        n=send(s2,buffer,strlen(buffer),0);
        }
}
