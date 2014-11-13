/*
 * squelette de serveur TCP démarré par inetd
 * un processus démarré pour chaque requête
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
        //réception sur S2 obtenue par accept!!!
        n=recv(s2,buffer,LEN,0);

        //émission sur S2 obtenue par accept!!!
        sprintf(buffer,"Lancement du serveur REUSSI %d\n",i);
        n=send(s2,buffer,strlen(buffer),0);
        }
}
