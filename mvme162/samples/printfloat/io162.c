/*
 * RP Thu Mar 25 10:02:08 CET 1999
 *
 * exemple d'utilisation des routines d'E/S texte de 162Bug
 */

#define LEN	80
char buf[LEN];

main()
{
while(1)
	{
	print("value : ");
	i=instr(LEN,buf);
	sscanf(buf+1,"%x",&i);
	
	sprintf(buf,"data = %x\n\r",i);
	print(buf);
	}
}
