/* time.c
 *
 * Mon Mar 20 14:59:17 CET 2006 RP
 *
 * afficher date / heure système
 * 	
m68k-linux-as crt0.s -o crt0.o

m68k-linux-gcc time.c -o time.s -S -03
m68k-linux-as time.s -o time.o
m68k-linux-ld crt0.o time.o -o time \
--oformat=srec -Ttext=0x10000 \
-lmvme162 -lc -L ..


 */
 
#define print(s) outstr(s, s+strlen(s))

#define secondes	0xFFFC1FF9	
#define minutes		0xFFFC1FFA	
#define heures		0xFFFC1FFB	
#define day		0xFFFC1FFC	
#define date		0xFFFC1FFD	
#define month		0xFFFC1FFE	
#define year		0xFFFC1FFF	


int main()
{
char str[50]="\012\015la date actuelle est ... ";
char buf[4];
char* pb;

pb=(char*) day;
switch(*pb) {
	case 1 : strcat(str, "Sunday "); break;
	case 2 : strcat(str, "Monday "); break;
	case 3 : strcat(str, "Tuesday "); break;
	case 4 : strcat(str, "Wednesday "); break;
	case 5 : strcat(str, "Thursday "); break;
	case 6 : strcat(str, "Friday "); break;
	case 7 : strcat(str, "Saturday "); break;
	}
	
pb=(char*) date;
buf[0]= (*pb >> 4)   + '0';
buf[1]= (*pb & 0x0F) + '0';
buf[2]='/';
buf[3]=0;
strcat(str,buf);

pb=(char*) month;
buf[0]= (*pb >> 4)   + '0';
buf[1]= (*pb & 0x0F) + '0';
buf[2]='/';
buf[3]=0;
strcat(str,buf);

pb=(char*) year;
buf[0]= (*pb >> 4)   + '0';
buf[1]= (*pb & 0x0F) + '0';
buf[2]=' ';
buf[3]=0;
strcat(str,buf);

pb=(char*) heures;
buf[0]= (*pb >> 4)   + '0';
buf[1]= (*pb & 0x0F) + '0';
buf[2]='H';
buf[3]=0;
strcat(str,buf);

pb=(char*) minutes;
buf[0]= (*pb >> 4)   + '0';
buf[1]= (*pb & 0x0F) + '0';
buf[2]=':';
buf[3]=0;

strcat(str,buf);
pb=(char*) secondes;
buf[0]= (*pb >> 4)   + '0';
buf[1]= (*pb & 0x0F) + '0';
buf[2]=0;
strcat(str,buf);

print(str);
print("\012\015bye... V0.0\012\015");
return 0;
}
